#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/transaction.hpp>
#include <optional>
using namespace eosio;
using namespace std;

CONTRACT events : public contract {
   public:
      using contract::contract;

      struct Metadata 
      {
         map<string, name> names;
         map<string, string> strings;
         map<string, asset> assets;
         map<string, time_point> time_points;
         map<string, uint64_t> ints;
         map<string, transaction> trxs;
      };

      struct Metric
      {
         uint64_t id;
         Metadata metadata;
      };

      struct [[eosio::table, eosio::contract("events")]] IntMetric
      {
         Metric metric;
         uint64_t value;
         uint64_t primary_key() const { return metric.id; }
      };
      typedef multi_index<"intmetrics"_n, IntMetric> int_metric_table;

      struct [[eosio::table, eosio::contract("events")]] AssetMetric
      {
         Metric metric;
         asset value;
         uint64_t primary_key() const { return metric.id; }
      };
      typedef multi_index<"assetmetrics"_n, AssetMetric> asset_metric_table;
      
      ACTION record (const name &scope, 
                     const map<string, name> names,
                     const map<string, string> strings,
                     const map<string, asset> assets,
                     const map<string, time_point> time_points,
                     const map<string, uint64_t> ints,
                     const map<string, transaction> trxs);

      ACTION increment ( const name &scope, const name &metric_name );
      ACTION addasset ( const name &scope, const name &metric_name, const asset &value );
      ACTION addint ( const name &scope, const name &metric_name, const uint64_t &value );

      ACTION newintmetric ( const name& owner, const name &metric_name, const optional<Metadata> &meta);
      ACTION newassetmtrc ( const name& owner, const name &metric_name, const optional<Metadata> &meta);
};