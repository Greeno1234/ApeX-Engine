#include "Zero.h"
#include "config.h"
#include "Unique.h"

#include <thread>

#ifndef SYS_DEBUG
  #ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
  #else
    #include <pthread.h>
    #include <semaphore.h>
  #endif
#endif

#include "Vector.h"

namespace sys
{

template <typename T>
struct ThreadPool
{
  ThreadPool()
  {
    size_t ideal = 1;

#ifndef SYS_DEBUG
    ideal = std::thread::hardware_concurrency();

    printf("Starting %i threads\n", (int)ideal);
    //std::cout << "Starting " << ideal << " threads" << std::endl;
#endif

    m_units.resize(ideal);

    for(typename Vector<Unit>::Iterator it =
      m_units.begin(); it != m_units.end(); ++it)
    {
      it->userdata = Unique<T>::make();
      it->tp = this;

#ifndef SYS_DEBUG
  #ifdef _WIN32
      it->start = CreateSemaphore(NULL, 0, 1, NULL);

      if(it->start == NULL)
      {
        throw std::exception();
      }

      it->stop = CreateSemaphore(NULL, 0, 1, NULL);

      if(it->stop == NULL)
      {
        throw std::exception();
      }

      it->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entry, (LPVOID)(void *)&(*it), 0, NULL);

      if(it->thread == NULL)
      {
        throw std::exception();
      }
  #else
      if(sem_init(&it->start, 0, 0) == -1)
      {
        throw std::exception();
      }

      if(sem_init(&it->stop, 0, 0) == -1)
      {
        throw std::exception();
      }

      if(pthread_create(&it->thread, NULL, entry,
        (void *)&(*it)) != 0)
      {
        it->thread = pthread_self();
        throw std::exception();
      }
  #endif
#endif
    }
  }

  ~ThreadPool()
  {
    m_func = NULL;

#ifndef SYS_DEBUG
    for(typename Vector<Unit>::Iterator it =
      m_units.begin(); it != m_units.end(); ++it)
    {
  #ifdef _WIN32
      ReleaseSemaphore(it->start, 1, NULL);
      WaitForSingleObject(it->thread, INFINITE);
      CloseHandle(it->thread);
      CloseHandle(it->start);
      CloseHandle(it->stop);
  #else
      sem_post(&it->start);
      pthread_join(it->thread, NULL);
      sem_destroy(&it->start);
      sem_destroy(&it->stop);
  #endif
    }
#endif
  }

  size_t size() const
  {
    return m_units.size();
  }

/*
  Ptr<T> at(size_t idx)
  {
    return m_units[idx].userdata;
  }
*/

  Ptr<T> at(size_t idx) const
  {
    return m_units[idx].userdata;
  }

  void execute(void (*_func)(T&))
  {
    m_func = _func;

#ifdef SYS_DEBUG
    _func(*m_units[0].userdata);
#else
    for(typename Vector<Unit>::Iterator it =
      m_units.begin(); it != m_units.end(); ++it)
    {
  #ifdef _WIN32
      ReleaseSemaphore(it->start, 1, NULL);
  #else
      sem_post(&it->start);
  #endif
    }

    for(typename Vector<Unit>::Iterator it =
      m_units.begin(); it != m_units.end(); ++it)
    {
  #ifdef _WIN32
      WaitForSingleObject(it->stop, INFINITE);
  #else
      sem_wait(&it->stop);
  #endif
    }
#endif

    m_func = NULL;
  }

private:
  struct Unit
  {
    Unique<T> userdata;
#ifndef SYS_DEBUG
  #ifdef _WIN32
    HANDLE thread;
    HANDLE start;
    HANDLE stop;
  #else
    pthread_t thread;
    sem_t start;
    sem_t stop;
  #endif
#endif
    ThreadPool *tp;
  };

  ThreadPool(const ThreadPool& _copy);
  ThreadPool& operator=(const ThreadPool& _other);

  Vector<Unit> m_units;
  Zero<void (*)(T&)> m_func;

#ifndef SYS_DEBUG
  #ifdef _WIN32
  static DWORD WINAPI entry(LPVOID ptr)
  #else
  static void *entry(void *ptr)
  #endif
  {
    Unit *tu = (Unit*)ptr;

    while(true)
    {
  #ifdef _WIN32
      WaitForSingleObject(tu->start, INFINITE);
  #else
      sem_wait(&tu->start);
  #endif

      if(!tu->tp->m_func)
      {
        break;
      }
      else
      {
        // TODO: Zero<T> is awkward for function pointers.
        //static_cast<void (*)(T&)>(tu->tp->m_func)(*tu->userdata);
        void (*func)(T&) = tu->tp->m_func;
        func(*tu->userdata);
      }

  #ifdef _WIN32
      ReleaseSemaphore(tu->stop, 1, NULL);
  #else
      sem_post(&tu->stop);
  #endif
    }

    return NULL;
  }
#endif

};

}

