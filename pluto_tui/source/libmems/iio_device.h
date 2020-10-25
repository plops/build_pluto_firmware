// Copyright 2019 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBMEMS_IIO_DEVICE_H_
#define LIBMEMS_IIO_DEVICE_H_

#include <iio.h>

#include <memory>
#include <string>
#include <vector>

#include <base/containers/flat_map.h>
#include <base/files/file_path.h>
#include <base/macros.h>
#include <base/optional.h>

#include "libmems/export.h"

namespace libmems {

class IioContext;
class IioChannel;

// The IioDevice represents a single IIO device, such as a gyroscope.
// It offers facilities to read and write attributes on the device, as well as
// configure channels, trigger and buffer for a sensor.
class LIBMEMS_EXPORT IioDevice {
 public:
  // first is channel index; second is the channel's value
  using IioSample = base::flat_map<int32_t, int64_t>;

  virtual ~IioDevice();

  // Returns the IIO context that contains this device.
  virtual IioContext* GetContext() const = 0;

  // Returns the value of the 'name' attribute of this device.
  // It is allowed to return an empty string.
  virtual const char* GetName() const = 0;

  // Returns the unique IIO identifier of this device/trigger.
  // Return id greater or equal to 0 if it's a device.
  // Return id be -1 if it's iio_sysfs_trigger, greater or equal to 0 if it's a
  // trigger.
  virtual int GetId() const = 0;

  // This call is used to enable setting UNIX permissions and ownership on the
  // attributes of a sensor. It should not be used as a replacement for the
  // read/write attribute accessors below.
  virtual base::FilePath GetPath() const = 0;

  // Reads the |name| attribute of this device and returns the value
  // as a string. It will return base::nullopt if the attribute cannot
  // be read.
  virtual base::Optional<std::string> ReadStringAttribute(
      const std::string& name) const = 0;

  // Reads the |name| attribute of this device and returns the value
  // as a signed number. It will return base::nullopt if the attribute
  // cannot be read or is not a valid number.
  virtual base::Optional<int64_t> ReadNumberAttribute(
      const std::string& name) const = 0;

  // Reads the |name| attribute of this device and returns the value
  // as a double precision floating point. It will return base::nullopt
  // if the attribute cannot be read or is not a valid number.
  virtual base::Optional<double> ReadDoubleAttribute(
      const std::string& name) const = 0;

  // Writes the string |value| to the attribute |name| of this device. Returns
  // false if an error occurs.
  virtual bool WriteStringAttribute(const std::string& name,
                                    const std::string& value) = 0;

  // Writes the number |value| to the attribute |name| of this device. Returns
  // false if an error occurs.
  virtual bool WriteNumberAttribute(const std::string& name, int64_t value) = 0;

  // Writes the floating point |value| to the attribute |name| of this device.
  // Returns false if an error occurs.
  virtual bool WriteDoubleAttribute(const std::string& name, double value) = 0;

  // Returns true if this device represents a single sensor, vs. a device
  // representing all available cros_ec sensors on the system, as defined
  // before 3.18 kernel.
  bool IsSingleSensor() const;

  // Returns the iio_device object underlying this object, if any is available.
  // Returns nullptr if no iio_device exists, e.g. a mock object.
  virtual iio_device* GetUnderlyingIioDevice() const = 0;

  // Sets |trigger| as the IIO trigger device for this device. It is expected
  // that |trigger| is owned by the same IIO context as this device.
  virtual bool SetTrigger(IioDevice* trigger_device) = 0;

  // Returns the IIO trigger device for this device, or nullptr if this device
  // has no trigger, or the trigger can't be found.
  virtual IioDevice* GetTrigger() = 0;

  // Returns all channels belonging to this device.
  std::vector<IioChannel*> GetAllChannels();

  // Finds the IIO channel |index| as the index in this device and returns it.
  // It will return nullptr if no such channel can be found.
  IioChannel* GetChannel(int32_t index);

  // Finds the IIO channel |name| as id or name for this device and returns it.
  // It will return nullptr if no such channel can be found.
  IioChannel* GetChannel(const std::string& name);

  // Returns the sample size in this device.
  // Returns base::nullopt on failure.
  virtual base::Optional<size_t> GetSampleSize() const = 0;

  // Enables the IIO buffer on this device and configures it to return
  // |num| samples on access. This buffer's lifetime can exceed that of the
  // IioContext, and that it's caller responsibility to know when to let go of
  // the buffer with DisableBuffer if at all.
  // It should not be used along with GetBufferFd or ReadEvent.
  // Returns false on failure.
  virtual bool EnableBuffer(size_t num) = 0;

  // Disables the IIO buffer on this device. Returns false on failure.
  virtual bool DisableBuffer() = 0;

  // Returns true if the IIO buffer is enabled for this device.
  // If it is enabled, it sets |num| to the number of samples.
  virtual bool IsBufferEnabled(size_t* num = nullptr) const = 0;

  // Creates the IIO buffer if it doesn't exist, and gets the file descriptor to
  // poll for events. Returns base::nullopt on failure.
  // The buffer's lifetime is managed by the IioDevice, which will be disabled
  // when the IioDevice along with the IioContext gets destroyed. It should not
  // be used along with EnableBuffer.
  virtual base::Optional<int32_t> GetBufferFd() = 0;

  // Creates the IIO buffer if it doesn't exist, and reads & returns one sample.
  // Returns base::nullopt on failure.
  // The buffer's lifetime is managed by the IioDevice, which will be disabled
  // when the IioDevice along with the IioContext gets destroyed. It should not
  // be used along with EnableBuffer.
  virtual base::Optional<IioSample> ReadSample() = 0;

 protected:
  struct ChannelData {
    std::string chn_id;
    std::unique_ptr<IioChannel> chn;
  };

  static base::Optional<int> GetIdAfterPrefix(const char* id_str,
                                              const char* prefix);

  IioDevice() = default;
  std::vector<ChannelData> channels_;

 private:
  DISALLOW_COPY_AND_ASSIGN(IioDevice);
};

}  // namespace libmems

#endif  // LIBMEMS_IIO_DEVICE_H_
