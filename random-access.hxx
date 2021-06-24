#pragma once

#include <boost/signals2/signal.hpp>

namespace RandomAccess {

class RandomAccess
{
public:
  RandomAccess();
  ~RandomAccess();

  // Read a buffer at a specific offset.  Returns the buffer read.
  virtual std::span<uint8_t> read(off_t offset, off_t length) = 0;

  // Write a buffer at a specific offset.
  virtual void write(off_t offset, std::span<uint8_t> buffer) = 0;

  // Delete data at a specific offset.
  // Will truncate the file if offset + length is larger than the current file length.  Is otherwise a noop.
  virtual void del(off_t offset, off_t length) = 0;

  // Stat the storage.  The returned object should have its `st_size` field set.
  virtual struct stat stat() = 0;

  // Close the underlying file descriptor.
  virtual void close() = 0;

  // Unlink the underlying file.
  virtual void destroy() = 0;

  // True if the storage has been fully opened.
  bool opened() const { return _opened; }

  // True if the storage has been fully closed.
  bool closed() const { return !_opened; }

  // True if the storage has been fully destroyed.
  bool destroyed() const { return _destroyed; }

  // Emitted when the storage is fully opeend.
  // You do not need to wait for this event before doing any reads/writes.
  boost::signals2::signal<void(RandomAccess &)> onOpen;

  // Emitted when the storage is fully closed.
  boost::signals2::signal<void(RandomAccess &)> onClose;

  // Emitted when the storage is fully destroyed.
  boost::signals2::signal<void(RandomAccess &)> onDestroy;
};

std::unique_ptr<RandomAccess> File;
std::unique_ptr<RandomAccess> Memory;

} // namespace RandomAccess
