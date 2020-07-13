#include <monitor.hpp>

using namespace monitorspace; 

void monitor::setowners (const set<name>& authorized_owners) {
    config_table      config_s (get_self(), get_self().value);
   	Config c = config_s.get_or_create (get_self(), Config());	
    c.authorized_owners = authorized_owners;
    config_s.set(c, get_self());
}

void monitor::addowner (const name& authorized_owner) {
    config_table      config_s (get_self(), get_self().value);
   	Config c = config_s.get_or_create (get_self(), Config());	
    c.authorized_owners.insert(authorized_owner);
    config_s.set(c, get_self());
}

void monitor::addowners (const set<name>& authorized_owners) {
    config_table      config_s (get_self(), get_self().value);
   	Config c = config_s.get_or_create (get_self(), Config());	
    c.authorized_owners.insert(authorized_owners.begin(), authorized_owners.end());
    config_s.set(c, get_self());
}

void monitor::remowner (const name& authorized_owner) {
    config_table      config_s (get_self(), get_self().value);
   	Config c = config_s.get_or_create (get_self(), Config());	
    c.authorized_owners.erase(authorized_owner);
    config_s.set(c, get_self());
}