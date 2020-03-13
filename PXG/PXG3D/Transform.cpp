#include "Transform.h"
#include "Debug.h"

namespace PXG
{
	Transform::Transform(Quaternion rotation, Vector3 position, Vector3 scale)
	{
		this->rotation = rotation;
		this->position = position;
	}
	Transform::Transform()
	{
		scale = Vector3(1.0f, 1.0f, 1.0f);
	}
	inline Quaternion Transform::GetLocalRotation() const
	{
		return rotation;
	}
	Vector3 Transform::GetLocalPosition() const
	{
		return position;
	}

	void Transform::SetLocalPosition(Vector3 localPosition)
	{
		position = localPosition;
	}

	Vector3 Transform::GetPosition() const
	{
		if (parentTransform)
		{
			Vector3 result(glm::vec4(position.ToGLMVec3(), 1) * parentTransform->GetWorldTransform().Matrix);

			return result;
		}
		return position;
	}

	void Transform::translate(Vector3 translation)
	{
		position = position + translation;
	}

	void Transform::rotate(Vector3 axis, float angle)
	{
		rotation = Mathf::ToQuaternion(AxisAngle(angle, axis.x, axis.y, axis.z)) * rotation;
	}

	void Transform::Scale(Vector3 newScale)
	{
		scale = newScale;
	}

	Quaternion Transform::GetRotation() const
	{
		if (parentTransform)
		{
			return  GetLocalRotation() * parentTransform->GetRotation();
		}
		return GetLocalRotation();
	}


	Mat4 Transform::GetLocalTransform() const
	{
		glm::mat4 mat4Scale(
			glm::vec4(scale.x, 0, 0, 0),
			glm::vec4(0, scale.y, 0, 0),
			glm::vec4(0, 0, scale.z, 0),
			glm::vec4(0, 0, 0, 1)
		);

		glm::quat Quaternion = rotation.ToGLMQuat();

		glm::mat4 mat4Rotation = glm::toMat4(Quaternion);


		glm::mat4 mat4Position(
			glm::vec4(1.0f, 0, 0, 0),
			glm::vec4(0, 1.0f, 0, 0),
			glm::vec4(0, 0, 1.0f, 0),
			glm::vec4(position.ToGLMVec3(), 1.0f)
		);



		return Mat4(mat4Position * mat4Rotation * mat4Scale);
	}

	Mat4 Transform::GetWorldTransform() const
	{
		if (parentTransform)
		{
			return  GetLocalTransform() * parentTransform->GetWorldTransform();
		}

		return GetLocalTransform();
	}

	void Transform::SetParent(Transform * transform)
	{
		parentTransform = transform;
	}

	Transform * Transform::GetParent() const
	{
		return parentTransform;
	}

	Vector3 Transform::getScale() const
	{
		return scale;
	}
	Vector3 Transform::GetForward() const
	{
		auto result = GetWorldTransform().ToGLM() * glm::vec4(0,0,1,0) ;
		return Vector3(result.x,result.y,result.z).Normalized();
	}

	Vector3 Transform::GetRight() const
	{
		auto result = GetWorldTransform().ToGLM() * glm::vec4(1, 0, 0, 0);
		return Vector3(result.x, result.y, result.z).Normalized();
	}

	Vector3 Transform::GetUp() const
	{
		auto result = GetWorldTransform().ToGLM() * glm::vec4(0, 1, 0, 0);
		return Vector3(result.x, result.y, result.z).Normalized();
	}


}

