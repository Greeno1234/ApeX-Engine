#ifndef SYS_DIRECTORY_H
#define SYS_DIRECTORY_H

#include "String.h"
#include "Zero.h"
#include "Ptr.h"
#include "List.h"
#include "Exception.h"

#include <dirent.h>

namespace sys
{

struct Directory;

struct DirectoryEntry
{
  sys::String name() const
  {
    if(!m_entry)
    {
      sys::panic("Directory entry is invalid");
    }

    return m_entry->d_name;
  }

private:
  friend struct Directory;

  sys::Ptr<Directory> m_dir;
  sys::Zero<dirent*> m_entry;
};

struct Directory : sys::EnablePtr
{
  Directory(const sys::String& _path) :
    m_dir(opendir(_path.unsafe_raw()))
  {
    if(!m_dir)
    {
      throw sys::Exception(sys::String() + "Failed to open directory '" + _path + "'");
    }
  }

  ~Directory()
  {
    m_entries.clear();
    closedir(m_dir);
  }

  sys::Ptr<DirectoryEntry> read()
  {
    dirent* ent = NULL;

    while(true)
    {
      ent = readdir(m_dir);
      if(!ent) return sys::Ptr<DirectoryEntry>();
      if(sys::String(ent->d_name) == ".") continue;
      if(sys::String(ent->d_name) == "..") continue;
      break;
    }

    sys::Ptr<DirectoryEntry> rtn(m_entries.emplace());

    rtn->m_dir = this;
    rtn->m_entry = ent;

    return rtn;
  }

private:
  sys::Zero<DIR*> m_dir;
  sys::List<DirectoryEntry> m_entries;

  Directory(const Directory& _copy);
  Directory& operator=(const Directory& _other);
};

}

#endif

