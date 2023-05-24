#include "isystem.hpp"

ISystem::ISystem(const SystemPriority priority, const double m_updater_internal) : m_priority(priority),
                                                                                   m_updater_internal(m_updater_internal),
                                                                                   m_enabled(true),
                                                                                   m_needs_update(true),
                                                                                   m_reserved(0) {
}

ISystem::~ISystem() {}
