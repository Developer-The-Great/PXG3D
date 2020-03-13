#include "Quaternion.h"
#include "AxisAngle.h"


namespace PXG {

    Quaternion AxisAngle::ToQuaternion() const
    {
        Vector3 axis(x, y, z);
        axis.Normalize();
        float qx = axis.x * Mathf::Sin(glm::radians(angle/2.0f));
        float qy = axis.y * Mathf::Sin(glm::radians(angle / 2.0f));
        float qz = axis.z * Mathf::Sin(glm::radians(angle / 2.0f));
        float qw = Mathf::Cos(glm::radians(angle / 2.0f));

        return Quaternion(qw, qx, qy, qz).Normalized();
    }
}