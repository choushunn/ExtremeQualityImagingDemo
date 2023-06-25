#include "connxbase.h"
#include "connx.h"
#include "connxfacepaint.h"

COnnxBase::COnnxBase()
{

}
COnnxBase* COnnxBase::createInstance(const std::string& type, const std::string &model_path, bool isGPU) {
    if (type == "connx") {
        return new COnnx(model_path, isGPU);
    } else if (type == "facepaint") {
        return new COnnxFacePaint(model_path, isGPU);
    } else {
        return nullptr;
    }
}
