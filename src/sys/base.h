#ifndef SYS_OBJECT_H
#define SYS_OBJECT_H

#include "criticalpin.h"
#include "zero.h"
#include "weakpin.h"
#include "config.h"

namespace sys
{

struct base
{
  virtual ~base();

  weak_pin::lock weak_pin_lock() const;

#ifdef SYS_DEBUG
  critical_pin::lock critical_pin_lock() const;
#endif

private:
#ifdef SYS_DEBUG
  critical_pin m_criticalpin;
#endif
  weak_pin m_weakpin;
};

}

/* Implementation */

namespace sys
{

inline base::~base()
{ }

#ifdef SYS_DEBUG
inline critical_pin::lock base::critical_pin_lock() const
{
  return critical_pin::lock(m_criticalpin);
}
#endif

inline weak_pin::lock base::weak_pin_lock() const
{
  return weak_pin::lock(m_weakpin);
}

}

#endif

