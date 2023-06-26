#include "connx.h"
#include "connxnaf.h"
#include "connxfacepaint.h"

COnnx::COnnx()
{

}

COnnx* COnnx::createInstance(const std::string& type, const std::string &model_path, bool isGPU) {
    if (type == "connx") {
        return new COnnxNAF(model_path, isGPU);
    } else if (type == "facepaint") {
        return new COnnxFacePaint(model_path, isGPU);
    } else {
        return nullptr;
    }
}
