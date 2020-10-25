// Copyright 2019 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBMEMS_TEST_FAKES_H_
#define LIBMEMS_TEST_FAKES_H_

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include <base/callback.h>
#include <base/files/scoped_file.h>

#include "libmems/export.h"
#include "libmems/iio_channel.h"
#include "libmems/iio_context.h"
#include "libmems/iio_device.h"

namespace libmems {

namespace fakes {

constexpr double kFakeSamplingFrequency = 20.0;

constexpr char kFakeAccelChns[][10] = {"accel_x", "accel_y", "accel_z",
                                       "timestamp"};

constexpr int64_t kFakeAccelSamples[][4] = {
    {-35, 7273, -4507, 1576146547637646757LL},
    {-38, 7272, -4508, 1576146547686105167LL},
    {-33, 7268, -4512, 1576146547734874023LL},
    {-34, 7269, -4504, 1576146547786726064LL},
    {-34, 7268, -4507, 1576146547833537914LL},
    {-31, 7273, -4505, 1576146547883362585LL},
    {-37, 7268, -4513, 1576146547934823156LL},
    {-38, 7275, -4508, 1576146547983531441LL},
    {-33, 7275, -4505, 1576146548033991396LL},
    {-38, 7267, -4507, 1576146548088318767LL},
    {-31, 7267, -4510, 1576146548134533603LL},
    {-36, 7274, -4506, 1576146548186771158LL},
    {-34, 7269, -4513, 1576146548238155689LL},
    {-37, 7272, -4513, 1576146548286133465LL},
    {-39, 7272, -4513, 1576146548336164236LL},
    {-33, 7270, -4508, 1576146548385059679LL},
    {-31, 7272, -4508, 1576146548436017710LL},
    {-35, 7270, -4506, 1576146548484018687LL},
    {-33, 7270, -4508, 1576146548538176956LL},
    {-40, 7269, -4509, 1576146548583872808LL},
    {-32, 7272, -4505, 1576146548635528461LL},
    {-39, 7269, -4509, 1576146548684053162LL},
    {-40, 7268, -4504, 1576146548735750128LL},
    {-38, 7275, -4505, 1576146548784907050LL},
    {-31, 7270, -4507, 1576146548833993600LL},
    {-32, 7270, -4513, 1576146548884498169LL},
    {-38, 7273, -4512, 1576146548935454841LL},
    {-34, 7268, -4511, 1576146548983930691LL},
    {-39, 7272, -4513, 1576146549038354675LL},
    {-31, 7270, -4504, 1576146549088311239LL},
    {-33, 7267, -4504, 1576146549134915814LL},
    {-35, 7267, -4512, 1576146549188095471LL},
    {-34, 7273, -4506, 1576146549233478083LL},
    {-35, 7272, -4510, 1576146549283536871LL},
    {-36, 7275, -4510, 1576146549337812680LL},
    {-31, 7268, -4512, 1576146549383408902LL},
    {-32, 7266, -4504, 1576146549436766877LL},
    {-31, 7266, -4508, 1576146549488317082LL},
    {-35, 7274, -4510, 1576146549534105170LL},
    {-35, 7267, -4512, 1576146549583480220LL},
    {-32, 7270, -4505, 1576146549634549753LL},
    {-40, 7269, -4513, 1576146549685827215LL},
    {-36, 7270, -4509, 1576146549736114408LL},
    {-39, 7269, -4507, 1576146549787294361LL},
    {-34, 7275, -4508, 1576146549833679601LL},
    {-32, 7273, -4512, 1576146549884508806LL},
    {-39, 7270, -4506, 1576146549934510649LL},
    {-33, 7275, -4508, 1576146549986826232LL},
    {-32, 7274, -4510, 1576146550034265609LL},
    {-31, 7274, -4512, 1576146550087723517LL},
    {-37, 7269, -4509, 1576146550136676150LL},
    {-40, 7272, -4505, 1576146550185471987LL},
    {-32, 7274, -4505, 1576146550237360278LL},
    {-34, 7273, -4506, 1576146550285154743LL},
    {-37, 7266, -4510, 1576146550334400698LL},
    {-35, 7268, -4504, 1576146550383655864LL},
    {-31, 7271, -4513, 1576146550434743018LL},
    {-38, 7275, -4507, 1576146550485860328LL},
    {-33, 7273, -4509, 1576146550535665916LL},
    {-39, 7274, -4509, 1576146550584116729LL},
    {-37, 7275, -4505, 1576146550633930996LL},
    {-38, 7266, -4505, 1576146550687553120LL},
    {-39, 7271, -4513, 1576146550737380685LL},
    {-35, 7274, -4513, 1576146550783783035LL},
    {-34, 7270, -4507, 1576146550837586694LL},
    {-34, 7274, -4508, 1576146550886005896LL},
    {-33, 7270, -4505, 1576146550936765656LL},
    {-34, 7266, -4509, 1576146550986799436LL},
    {-40, 7275, -4504, 1576146551036790826LL},
    {-39, 7269, -4512, 1576146551084304842LL},
    {-36, 7269, -4513, 1576146551137261255LL},
    {-39, 7275, -4510, 1576146551187516034LL},
    {-37, 7275, -4507, 1576146551235906273LL},
    {-35, 7266, -4505, 1576146551287630187LL},
    {-40, 7270, -4507, 1576146551334310300LL},
    {-38, 7272, -4509, 1576146551386779464LL},
    {-34, 7271, -4506, 1576146551438113310LL},
    {-38, 7273, -4505, 1576146551485026553LL},
    {-31, 7275, -4511, 1576146551533460722LL},
    {-33, 7268, -4513, 1576146551585759710LL},
    {-38, 7269, -4512, 1576146551638235580LL},
    {-31, 7275, -4508, 1576146551683932695LL},
    {-39, 7275, -4509, 1576146551735875095LL},
    {-32, 7271, -4506, 1576146551785431314LL},
    {-32, 7270, -4513, 1576146551835168501LL},
    {-31, 7268, -4509, 1576146551888121294LL},
    {-36, 7266, -4512, 1576146551934178770LL},
    {-40, 7268, -4511, 1576146551986036666LL},
    {-31, 7268, -4508, 1576146552038004999LL},
    {-37, 7274, -4511, 1576146552084664014LL},
    {-40, 7270, -4513, 1576146552135709672LL},
    {-34, 7275, -4512, 1576146552188045583LL},
    {-36, 7270, -4508, 1576146552238295106LL},
    {-34, 7269, -4504, 1576146552285517753LL},
    {-38, 7266, -4508, 1576146552334026224LL},
    {-35, 7269, -4509, 1576146552383932953LL},
    {-37, 7270, -4506, 1576146552436224995LL},
    {-31, 7272, -4509, 1576146552487159335LL},
    {-34, 7268, -4511, 1576146552535876318LL},
    {-32, 7268, -4512, 1576146552586294158LL}};

class LIBMEMS_EXPORT FakeIioChannel : public IioChannel {
 public:
  FakeIioChannel(const std::string& id, bool enabled);

  const char* GetId() const override { return id_.c_str(); }

  bool IsEnabled() const override { return enabled_; }
  bool SetEnabled(bool en) override;

  base::Optional<std::string> ReadStringAttribute(
      const std::string& name) const override;
  base::Optional<int64_t> ReadNumberAttribute(
      const std::string& name) const override;
  base::Optional<double> ReadDoubleAttribute(
      const std::string& name) const override;

  bool WriteStringAttribute(const std::string& name,
                            const std::string& value) override;
  bool WriteNumberAttribute(const std::string& name, int64_t value) override;
  bool WriteDoubleAttribute(const std::string& name, double value) override;

  // |index| should be within [0, |base::size(kFakeAccelSamples)|). If the raw
  // attribute exists, the value is returned; otherwise, if the channel's id is
  // within kFakeAccelChns, the corresponding value in |kFakeAccelSamples| is
  // returned.
  // Returns base::nullopt if none of the above is true.
  base::Optional<int64_t> GetData(int index);

 private:
  std::string id_;
  bool enabled_;
  std::map<std::string, std::string> text_attributes_;
  std::map<std::string, int64_t> numeric_attributes_;
  std::map<std::string, double> double_attributes_;
};

class FakeIioContext;

class LIBMEMS_EXPORT FakeIioDevice : public IioDevice {
 public:
  FakeIioDevice(FakeIioContext* ctx, const std::string& name, int id);

  IioContext* GetContext() const override {
    return reinterpret_cast<IioContext*>(context_);
  }

  base::FilePath GetPath() const override;
  iio_device* GetUnderlyingIioDevice() const override { return nullptr; }

  const char* GetName() const override { return name_.c_str(); }
  int GetId() const override { return id_; }

  base::Optional<std::string> ReadStringAttribute(
      const std::string& name) const override;
  base::Optional<int64_t> ReadNumberAttribute(
      const std::string& name) const override;
  base::Optional<double> ReadDoubleAttribute(
      const std::string& name) const override;

  bool WriteStringAttribute(const std::string& name,
                            const std::string& value) override;
  bool WriteNumberAttribute(const std::string& name, int64_t value) override;
  bool WriteDoubleAttribute(const std::string& name, double value) override;

  bool SetTrigger(IioDevice* trigger) override;
  IioDevice* GetTrigger() override { return trigger_; }

  void AddChannel(std::unique_ptr<FakeIioChannel> chn) {
    channels_.push_back({chn->GetId(), std::move(chn)});
  }

  bool EnableBuffer(size_t n) override;
  bool DisableBuffer() override;
  bool IsBufferEnabled(size_t* n = nullptr) const override;

  base::Optional<size_t> GetSampleSize() const override {
    return base::nullopt;
  }

  base::Optional<int32_t> GetBufferFd() override;
  base::Optional<IioSample> ReadSample() override;

  // Simulates a bad device: not readable fd and fails all reads.
  void DisableFd();
  // Simulates some failures when reading the kth sample. Can be called multiple
  // times. The user should make sure the kth sample hasn't been read.
  void AddFailedReadAtKthSample(int k);

  // Pauses at kth sample. |callback| is run when (k-1)th sample is retrieved or
  // when this function is called and |k| == |sample_index_|.
  // The user should make sure that there wasn't a pause set and not occurred
  // yet, |k| doesn't exceeds fake data's size, and the kth sample hasn't been
  // read.
  void SetPauseCallbackAtKthSamples(int k, base::OnceCallback<void()> callback);
  // Resumes reading after being paused.
  // The user should make sure this device is paused.
  void ResumeReadingSamples();

 private:
  struct ChannelData {
    std::string chn_id;
    FakeIioChannel* chn;
  };

  bool CreateBuffer();
  bool WriteByte();
  bool ReadByte();
  void ClosePipe();
  void SetPause();

  FakeIioContext* context_;
  std::string name_;
  int id_;
  std::map<std::string, std::string> text_attributes_;
  std::map<std::string, int64_t> numeric_attributes_;
  std::map<std::string, double> double_attributes_;
  IioDevice* trigger_;

  // For |EnableBuffer|, |DisableBuffer|, and |IsBufferEnabled|.
  size_t buffer_length_ = 0;
  bool buffer_enabled_ = false;

  // For |CreateBuffer|, |GetBufferFd|, and |ReadSample|.
  base::ScopedFD sample_fd_;
  bool readable_fd_ = false;
  int sample_index_ = 0;

  // Pops from the failure with the smallest sample index.
  std::priority_queue<int, std::vector<int>, std::greater<int>>
      failed_read_queue_;

  bool disabled_fd_ = false;
  bool is_paused_ = false;
  base::Optional<int> pause_index_;
  base::OnceCallback<void()> pause_callback_;
};

class LIBMEMS_EXPORT FakeIioContext : public IioContext {
 public:
  FakeIioContext() = default;

  void AddDevice(std::unique_ptr<FakeIioDevice> device);
  void AddTrigger(std::unique_ptr<FakeIioDevice> trigger);

  iio_context* GetCurrentContext() const override { return nullptr; };
  void Reload() override {}
  bool SetTimeout(uint32_t timeout) override {
    timeout_ = timeout;
    return true;
  }

  std::vector<IioDevice*> GetDevicesByName(const std::string& name) override;
  IioDevice* GetDeviceById(int id) override;
  std::vector<IioDevice*> GetAllDevices() override;

  std::vector<IioDevice*> GetTriggersByName(const std::string& name) override;
  IioDevice* GetTriggerById(int id) override;
  std::vector<IioDevice*> GetAllTriggers() override;

 private:
  IioDevice* GetFakeById(
      int id, const std::map<int, std::unique_ptr<FakeIioDevice>>& devices_map);
  std::vector<IioDevice*> GetFakeByName(
      const std::string& name,
      const std::map<int, std::unique_ptr<FakeIioDevice>>& devices_map);
  std::vector<IioDevice*> GetFakeAll(
      const std::map<int, std::unique_ptr<FakeIioDevice>>& devices_map);

  std::map<int, std::unique_ptr<FakeIioDevice>> devices_;
  std::map<int, std::unique_ptr<FakeIioDevice>> triggers_;

  uint32_t timeout_;
};

}  // namespace fakes

}  // namespace libmems

#endif  // LIBMEMS_TEST_FAKES_H_
