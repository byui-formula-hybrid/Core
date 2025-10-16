#pragma once

namespace Core {

class iEncoder;
enum class EncodeError;

class iEncodable {
  public: 
    virtual EncodeError encode(iEncoder& encoder) const = 0;
    
    virtual ~iEncodable() = default;
};

} // namespace Core