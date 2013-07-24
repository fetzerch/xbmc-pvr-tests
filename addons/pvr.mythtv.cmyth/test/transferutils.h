#ifndef TRANSFERUTILS_H
#define TRANSFERUTILS_H

template <class T>
class TransferUtil
{
public:
  void Add(const T *entry)
  {
    m_data.push_back(*entry);
  }

  std::list<T> GetList() const
  {
    return m_data;
  }

private:
  std::list<T> m_data;
};

#endif // TRANSFERUTILS_H
