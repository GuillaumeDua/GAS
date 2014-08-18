#ifndef __MEMORY_TOOLS__
# define __MEMORY_TOOLS__

// dest need to be a free-space area
template <typename T>
void	SecurePtrCopy_freeArea(T *& dest, const T * src)
{
  if (src != 0x0)
    {
      dest = new T;
      memcpy(dest, src, sizeof(T));
    }
  else dest = 0x0;
}

// cost
template <typename T>
void	SecurePtrCopy(T *& dest, const T * src)
{
  if (dest != 0x0) delete dest;

  if (src != 0x0)
    {
      dest = new T;
      memcpy(dest, src, sizeof(T));
    }
  else dest = 0x0;
}

#endif // __MEMORY_TOOLS__
