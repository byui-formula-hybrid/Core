#pragma once

#include "../data_model.h"
#include "../pedal_errors.h"

namespace Dash {
namespace UI {

inline const char* pedal_error_to_text(const Dash::DataModel& data) {
    if (data.pedal_error_code > 0) {
        return Dash::PedalErrors::decode(data.pedal_error_code);
    }

    if (data.pedal_sync_fault) {
        return "Pedal Implausible";
    }

    return "";
}

}  // namespace UI
}  // namespace Dash
