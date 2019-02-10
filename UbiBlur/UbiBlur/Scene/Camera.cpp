//
//  Camera.cpp
//  Engine
//
//  Created by Pavlo Muratov on 24.03.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace Engine {

#pragma mark - Lifecycle

    Camera::Camera()
            : mPosition(glm::zero<glm::vec3>()),
              mFieldOfView(75),
              mNearClipPlane(0.1),
              mFarClipPlane(10),
              mViewportAspectRatio(16.f / 9.f),
              mWorldUp(glm::vec3(0, 1, 0)),
              mFront(glm::vec3(0, 0, 1)),
              mRight(glm::vec3(1, 0, 0)),
              mUp(glm::vec3(0, 1, 0)),
              mPitch(0),
              mYaw(-90.f),
              mMaximumPitch(85.f) {}

    Camera::Camera(float fieldOfView, float zNear, float zFar) : Camera() {
        mFieldOfView = fieldOfView;
        mNearClipPlane = zNear;
        mFarClipPlane = zFar;
        updateVectors();
    }

#pragma mark - Private methods

    void Camera::updateVectors() {
        mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront.y = sin(glm::radians(mPitch));
        mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mFront = glm::normalize(mFront);
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp = glm::normalize(glm::cross(mRight, mFront));
    }

#pragma mark - Public methods

    void Camera::moveTo(const glm::vec3 &position) {
        mPosition = position;
    }

    void Camera::moveBy(const glm::vec3 &translation) {
        mPosition += translation;
    }

    void Camera::lookAt(const glm::vec3 &point) {
        glm::vec3 direction = glm::normalize(point - mPosition);
        mPitch = glm::degrees(asin(-direction.y));
        mYaw = glm::degrees(atan2(direction.x, direction.z)) + 90;

        updateVectors();
    }

    void Camera::rotateTo(float pitch, float yaw) {
        mPitch = 0.f;
        mYaw = 0.f;

        rotateBy(pitch, yaw);
    }

    void Camera::rotateBy(float pitch, float yaw) {
        mPitch += pitch;
        mYaw += yaw;

        if (mPitch > mMaximumPitch) {
            mPitch = mMaximumPitch;
        }
        if (mPitch < -mMaximumPitch) {
            mPitch = -mMaximumPitch;
        }

        updateVectors();
    }

    void Camera::zoom(float zoomFactor) {

    }

    void Camera::setFarPlane(float farPlane) {
        mFarClipPlane = std::max(mNearClipPlane, farPlane);
    }

    glm::vec3 Camera::worldToNDC(const glm::vec3 &v) const {
        glm::vec4 clipSpaceVector = viewProjectionMatrix() * glm::vec4(v, 1.0);
        return fabs(clipSpaceVector.w) > std::numeric_limits<float>::epsilon() ? clipSpaceVector / clipSpaceVector.w : clipSpaceVector;
    }

#pragma mark - Getters

    const glm::vec3 &Camera::position() const {
        return mPosition;
    }

    const glm::vec3 &Camera::front() const {
        return mFront;
    }

    const glm::vec3 &Camera::right() const {
        return mRight;
    }

    const glm::vec3 &Camera::up() const {
        return mUp;
    }

    float Camera::nearClipPlane() const {
        return mNearClipPlane;
    }

    float Camera::farClipPlane() const {
        return mFarClipPlane;
    }

    float Camera::FOVH() const {
        return mFieldOfView;
    }

    float Camera::FOVV() const {
        return mFieldOfView / mViewportAspectRatio;
    }

    glm::mat4 Camera::viewProjectionMatrix() const {
        return projectionMatrix() * viewMatrix();
    }

    glm::mat4 Camera::viewMatrix() const {
        return glm::lookAt(mPosition, mPosition + mFront, mWorldUp);
    }

    glm::mat4 Camera::projectionMatrix() const {
        float fovV = mFieldOfView / mViewportAspectRatio;
        return glm::perspective(glm::radians(fovV), mViewportAspectRatio, mNearClipPlane, mFarClipPlane);
    }

    glm::mat4 Camera::inverseViewProjectionMatrix() const {
        return glm::inverse(viewProjectionMatrix());
    }

    glm::mat4 Camera::inverseViewMatrix() const {
        return glm::inverse(viewMatrix());
    }

    glm::mat4 Camera::inverseProjectionMatrix() const {
        return glm::inverse(projectionMatrix());
    }

#pragma mark - Setters

    void Camera::setViewportAspectRatio(float aspectRatio) {
        mViewportAspectRatio = aspectRatio;
    }

}
