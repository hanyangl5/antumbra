#include <spirv_cross.hpp>

#include "shader_reflection.h"

namespace ant::gal {

ShaderReflection ReflectFromSpirv(CompiledShader *shader_blob);

}