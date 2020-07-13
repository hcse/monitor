#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <optional>
using namespace eosio;
using namespace std;

namespace monitorspace
{
   CONTRACT monitor : public contract
   {
   public:
      monitor(name self, name code, datastream<const char *> ds);
      ~monitor();

      typedef std::variant<int64_t, asset> metric_value;
      typedef std::variant<name, string, asset, time_point, int64_t> metadata_value;

      struct Metadata
      {
         // map<string, name> names;
         // map<string, string> strings;
         // map<string, asset> assets;
         // map<string, time_point> time_points;
         // map<string, int64_t> ints;
         map<string, metadata_value> values;
      };

      struct [[eosio::table, eosio::contract("monitor")]] Config
      {
         set<name> authorized_owners;
         Metadata metadata;
      };

      typedef singleton<name("config"), Config> config_table;
      typedef multi_index<name("config"), Config> config_table_placeholder;

      // scope: metric_name
      struct [[eosio::table, eosio::contract("monitor")]] TimeSeries
      {
         time_point timestamp;
         metric_value delta;
         string annotation;
         uint64_t primary_key() const { return timestamp.time_since_epoch().count(); }

         EOSLIB_SERIALIZE(TimeSeries, (timestamp)(delta)(annotation))
      };
      typedef multi_index<name("timeseries"), TimeSeries> time_series_table;

      // scope: get_self()
      struct [[eosio::table, eosio::contract("monitor")]] Metric
      {
         name metric_name;
         Metadata metadata;
         metric_value value;

         uint64_t primary_key() const { return metric_name.value; }

         metric_value get_value() const
         {
            return std::visit(
                [](auto &&arg) -> metric_value {
                   return arg;
                },
                value);
         }

         metric_value increment() const
         {
            if (std::holds_alternative<int64_t>(value))
            {
               return std::get<int64_t>(value) + 1;
            }
            else if (std::holds_alternative<asset>(value))
            {
               return asset{std::get<asset>(value).amount + 1, std::get<asset>(value).symbol};
            }
            check(false, "Fatal error. Metric increment does not support this variant type.");
            return -1;
         }

         metric_value add(metric_value operand) const
         {
            if (std::holds_alternative<int64_t>(value))
            {
               check(std::holds_alternative<int64_t>(operand), "Error: can only add an int64 value to metric name: " + metric_name.to_string());
               return std::get<int64_t>(value) + std::get<int64_t>(operand);
            }
            else if (std::holds_alternative<asset>(value))
            {
               check(std::holds_alternative<asset>(operand), "Error: can only add an asset value to metric name: " + metric_name.to_string());
               return std::get<asset>(value) + std::get<asset>(operand);
            }
            check(false, "Fatal error. Metric add function does not support this variant type.");
            return -1;
         }
         EOSLIB_SERIALIZE(Metric, (metric_name)(metadata)(value))
      };
      typedef multi_index<name("metrics"), Metric> metric_table;

      ACTION newmetric(const name &owner, const name &metric_name, const optional<Metadata> &meta);
      ACTION setvalue(const name &metric_name, const metric_value &value);
      ACTION increment(const name &metric_name);
      ACTION add(const name &metric_name, const metric_value &operand);
      ACTION track(const name &metric_name);
      ACTION untrack(const name &metric_name);
      ACTION setmetadata(const name &metric_name, const string &key, const metadata_value &value);

      ACTION toggletrack(const name &metric_name);
      ACTION annotate(const name &metric_name, const time_point &timestamp, const string &annotation);

      ACTION setowners(const set<name> &authorized_owners);
      ACTION addowners(const set<name> &authorized_owners);
      ACTION addowner(const name &authorized_owner);
      ACTION remowner(const name &authorized_owner);

   private:
      bool is_tracked(const name &metric_name);
      void trackdelta(const name &metric_name, const time_point &timestamp, const metric_value &delta);
   };

} // namespace monitorspace