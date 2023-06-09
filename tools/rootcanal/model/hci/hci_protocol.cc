//
// Copyright 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "hci_protocol.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "log.h"

namespace rootcanal {

size_t HciProtocol::WriteSafely(AsyncDataChannel* socket, const uint8_t* data,
                                size_t length) {
  size_t transmitted_length = 0;
  while (length > 0) {
    ssize_t ret = socket->Send(data + transmitted_length, length);

    if (ret == -1) {
      if (errno == EAGAIN) continue;
      LOG_ERROR("%s error writing to UART (%s)", __func__, strerror(errno));
      break;

    } else if (ret == 0) {
      // Nothing written :(
      LOG_ERROR("%s zero bytes written - something went wrong...", __func__);
      break;
    }

    transmitted_length += ret;
    length -= ret;
  }

  return transmitted_length;
}

}  // namespace rootcanal
