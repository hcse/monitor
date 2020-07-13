#include "monitor.hpp"

using namespace monitorspace; 

monitor::monitor(name self, name code, datastream<const char*> ds) : contract(self, code, ds) {}

monitor::~monitor() {}

void monitor::newmetric ( const name& owner, const name &metric_name, const optional<map<string, metadata_value>> &meta  ) {
    require_auth (owner);
    metric_table m_t (get_self(), get_self().value);
    m_t.emplace (owner, [&](auto &m) {
        m.metric_name = metric_name;
        m.metadata = meta.value();
        m.metadata["owner"] = owner;
        m.metadata["created_date"] = current_time_point();
        m.metadata["updated_date"] = current_time_point();
    });
}

void monitor::annotate (const name& metric_name, const time_point &timestamp, const string &annotation ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    time_series_table ts_t (get_self(), metric_name.value);
    auto ts_itr = ts_t.find (timestamp.time_since_epoch().count());
    check (ts_itr != ts_t.end(), "Cannot annotate. A time-series entry does not exist for this metric and timestamp: " + 
        metric_name.to_string() + " timestamp (microseconds): " + std::to_string(timestamp.time_since_epoch().count()));

    ts_t.modify (ts_itr, get_self(), [&](auto &ts) {
        ts.annotation = annotation;
    });
}

void monitor::trackdelta ( const name& metric_name, const time_point &timestamp, const metric_value &delta ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    time_series_table ts_t (get_self(), metric_name.value);
    auto ts_itr = ts_t.find (timestamp.time_since_epoch().count());
    check (ts_itr == ts_t.end(), "A time-series entry already exists for this metric and timestamp: " + 
        metric_name.to_string() + " timestamp (microseconds): " + std::to_string(timestamp.time_since_epoch().count()));

    ts_t.emplace (get_self(), [&](auto &ts) {
        ts.timestamp = timestamp;
        ts.delta = delta;        
    });
}

bool monitor::is_tracked ( const name& metric_name ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    if (m_itr->metadata.find("track") != m_itr->metadata.end() && 
        std::get<int64_t>(m_itr->metadata.at("track")) == 1) {
        return true;
    }
    return false;
}

void monitor::toggletrack ( const name& metric_name) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    m_t.modify (m_itr, get_self(), [&](auto &m) {
        if (is_tracked(metric_name)) {
            m.metadata["track"] = 0;
        } else {
            m.metadata["track"] = 1;
        }
    });
}

void monitor::track ( const name& metric_name) {
    if (is_tracked(metric_name)) { return; } 
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    m_t.modify (m_itr, get_self(), [&](auto &m) {
    //    m.metadata.ints["track"] = 1;
       m.metadata["track"] = 1;
    //    m.metadata.time_points["updated_date"] = current_time_point();       
       m.metadata["updated_date"] = current_time_point();
    });
}

void monitor::untrack ( const name& metric_name) {
    if (!is_tracked(metric_name)) { return; } 
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    m_t.modify (m_itr, get_self(), [&](auto &m) {
       m.metadata["track"] = 0;
       m.metadata["updated_date"] = current_time_point();
    });
}

void monitor::setmetadata (const name& metric_name, const string& key, const metadata_value& value) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    m_t.modify (m_itr, get_self(), [&](auto &m) {
       m.metadata[key] = value;
    });
}

void monitor::setvalue ( const name& metric_name, const metric_value &value ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    m_t.modify (m_itr, get_self(), [&](auto &m) {
        m.value = value;
        m.metadata["updated_date"] = current_time_point();
    });
}

void monitor::increment ( const name& metric_name ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));

    time_point timestamp = current_time_point();
    m_t.modify (m_itr, get_self(), [&](auto &m) {
        m.value = m_itr->increment();
        m.metadata["updated_date"] = timestamp;
    });

    if (is_tracked(metric_name)) {
        trackdelta (metric_name, timestamp, 1);
    }
}

void monitor::add (const name& metric_name, const metric_value &operand) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (std::get<name>(m_itr->metadata.at("owner")));
    time_point timestamp = current_time_point();
    m_t.modify (m_itr, get_self(), [&](auto &m) {
        m.value = m_itr->add(operand);
        m.metadata["updated_date"] = timestamp;
    });

    if (is_tracked(metric_name)) {
        trackdelta (metric_name, timestamp, operand);
    }
}