#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // TODO: (Req 3) Write this function
        // Create a scale matrix with the given scale values
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        // Create a rotation matrix using the yaw, pitch, and roll values
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        // Create a translation matrix with the given position values
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

        // Multiply the matrices in the correct order to obtain the final transformation matrix
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

    void Transform::set(glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale)
    {
        this->position = position;
        this->rotation = glm::radians(rotation);
        this->scale = scale;
    }

}