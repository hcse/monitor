#include <events.hpp>

void events::record (const name &scope, 
                     const map<string, name> names,
                     const map<string, string> strings,
                     const map<string, asset> assets,
                     const map<string, time_point> time_points,
                     const map<string, uint64_t> ints,
                     const map<string, transaction> trxs) {}

void events::increment ( const name &scope, const name &metric_name ) {}

void events::addasset ( const name &scope, const name &metric_name, const asset &value ) {}

void events::addint ( const name &scope, const name &metric_name, const uint64_t &value ) {}

void events::newintmetric ( const name& owner, const name &metric_name, const optional<Metadata> &meta ) {
    require_auth (owner);
    int_metric_table im_t (get_self(), get_self().value);
    im_t.emplace (get_self(), [&](auto &m) {
        m.metric.id = im_t.available_primary_key();
        m.metric.metadata = meta.value();
        m.metric.metadata.names["owner"] = owner;
    });
}

void events::newmetric ( const name& owner, const name &metric_name, const optional<Metadata> &meta  ) {}