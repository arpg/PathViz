#ifndef PATHVIZ_IMAGE_H
#define PATHVIZ_IMAGE_H

#include <pathviz/Types.h>

namespace pathviz
{

class Image
{
  public: Image() :
      m_width(0),
      m_height(0),
      m_data(nullptr),
      m_bytesAllocated(0),
      m_owner(false)
    {
    }

  public: Image(uint width, uint height) :
      m_width(0),
      m_height(0),
      m_data(nullptr),
      m_bytesAllocated(0),
      m_owner(false)
    {
      Resize(width, height);
    }

  public: Image(uint width, uint height, uchar* data) :
      m_width(width),
      m_height(height),
      m_data(data),
      m_bytesAllocated(bytes()),
      m_owner(false)
    {
    }

  public: ~Image()
    {
      if (m_owner)
      {
        delete m_data;
      }
    }

  public: inline void Resize(uint width, uint height)
    {
      m_width = width;
      m_height = height;
      uint newBytes = bytes();

      // check if new allocation needed
      if (newBytes > m_bytesAllocated)
      {
        // check if pointer owner
        if (m_owner)
        {
          delete m_data;
        }

        m_data = new uchar[newBytes];
        m_owner = true;
      }
    }

  public: inline uint width() const
    {
      return m_width;
    }

  public: inline uint height() const
    {
      return m_height;
    }

  public: inline uchar* data() const
    {
      return m_data;
    }

  public: inline uint bytes() const
    {
      uint bytesPerPixel = 4 * sizeof(uchar);
      return m_width * m_height * bytesPerPixel;
    }

  protected: uint m_width;

  protected: uint m_height;

  protected: uchar* m_data;

  protected: uint m_bytesAllocated;

  protected: bool m_owner;
};

}

#endif