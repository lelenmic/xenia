/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <xenia/kernel/modules/xboxkrnl/fs/devices/host_path_entry.h>

#include <xenia/kernel/modules/xboxkrnl/fs/devices/host_path_file.h>


using namespace xe;
using namespace xe::kernel;
using namespace xe::kernel::xboxkrnl;
using namespace xe::kernel::xboxkrnl::fs;


namespace {


class HostPathMemoryMapping : public MemoryMapping {
public:
  HostPathMemoryMapping(uint8_t* address, size_t length, xe_mmap_ref mmap) :
      MemoryMapping(address, length) {
    mmap_ = xe_mmap_retain(mmap);
  }
  virtual ~HostPathMemoryMapping() {
    xe_mmap_release(mmap_);
  }
private:
  xe_mmap_ref mmap_;
};


}


HostPathEntry::HostPathEntry(Type type, Device* device, const char* path,
                     const xechar_t* local_path) :
    Entry(type, device, path) {
  local_path_ = xestrdup(local_path);
}

HostPathEntry::~HostPathEntry() {
  xe_free(local_path_);
}

MemoryMapping* HostPathEntry::CreateMemoryMapping(
    xe_file_mode file_mode, const size_t offset, const size_t length) {
  xe_mmap_ref mmap = xe_mmap_open(file_mode, local_path_, offset, length);
  if (!mmap) {
    return NULL;
  }

  HostPathMemoryMapping* lfmm = new HostPathMemoryMapping(
      (uint8_t*)xe_mmap_get_addr(mmap), xe_mmap_get_length(mmap),
      mmap);
  xe_mmap_release(mmap);

  return lfmm;
}

X_STATUS HostPathEntry::Open(
    KernelState* kernel_state,
    XFile** out_file) {
  //*out_file = new DiscImageFile...
  return X_STATUS_NOT_IMPLEMENTED;
}
