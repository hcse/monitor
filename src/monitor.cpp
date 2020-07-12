#include <monitor.hpp>

void monitor::newmetric ( const name& owner, const name &metric_name, const optional<Metadata> &meta  ) {
    require_auth (owner);
    metric_table m_t (get_self(), get_self().value);
    m_t.emplace (owner, [&](auto &m) {
        m.metric_name = metric_name;
        m.metadata = meta.value();
        m.metadata.names["owner"] = owner;
        m.metadata.time_points["created_date"] = current_time_point();
        m.metadata.time_points["updated_date"] = current_time_point();
    });
}

void monitor::setvalue ( const name& metric_name, const metric_value &value ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (m_itr->metadata.names.at("owner"));

    m_t.modify (m_itr, m_itr->metadata.names.at("owner"), [&](auto &m) {
        m.value = value;
        m.metadata.time_points["updated_date"] = current_time_point();
    });
}

void monitor::increment ( const name& metric_name ) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (m_itr->metadata.names.at("owner"));
    m_t.modify (m_itr, m_itr->metadata.names.at("owner"), [&](auto &m) {
        m.value = m_itr->increment();
        m.metadata.time_points["updated_date"] = current_time_point();
    });
}

void monitor::add (const name& metric_name, const metric_value &operand) {
    metric_table m_t (get_self(), get_self().value);
    auto m_itr  = m_t.find (metric_name.value);
    check (m_itr != m_t.end(), "Metric with name not found: " + metric_name.to_string());

    require_auth (m_itr->metadata.names.at("owner"));
    m_t.modify (m_itr, m_itr->metadata.names.at("owner"), [&](auto &m) {
        m.value = m_itr->add(operand);
        m.metadata.time_points["updated_date"] = current_time_point();
    });
}