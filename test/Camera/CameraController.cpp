#include "CameraController.h"

#include <corecrt_math_defines.h>

#include "../Game.h"

CameraController::CameraController()
{
    
}

CameraController::CameraController(Camera* cam, Game* _game)
{
    this->game=_game;
    this->camera=cam;
    game->wInput->RawOffsetEvent.AddRaw(this,&CameraController::RawInput);
}

void CameraController::RawInput(POINT p)
{
    if (game->wInput->IsKeyDown(Keys::RightButton))
        this->camera->AdjustRotation(p.y*0.01f,p.x*0.01f,0.0f);
}


void CameraController::CameraMovement(float deltaSec)
{
    using namespace DirectX;
    int index=-1;
    if (game->wInput->IsKeyDown(Keys::W))
    {
        this->camera->AdjustPosition(this->camera->GetForwardVector()*cameraSpeed*deltaSec);
    }
    if (game->wInput->IsKeyDown(Keys::A))
    {
        this->camera->AdjustPosition(this->camera->GetLeftVector()*cameraSpeed*deltaSec);
    }
    if (game->wInput->IsKeyDown(Keys::S))
    {
        this->camera->AdjustPosition( this->camera->GetBackwardVector()*cameraSpeed*deltaSec);
    }
    if (game->wInput->IsKeyDown(Keys::D))
    {
        this->camera->AdjustPosition(this->camera->GetRightVector()*cameraSpeed*deltaSec);
    }
    if (game->wInput->IsKeyDown(Keys::Space))
    {
        this->camera->AdjustPosition(0.0f,cameraSpeed*deltaSec,0.0f);
    }
    if (game->wInput->IsKeyDown(Keys::LeftShift))
    {
        this->camera->AdjustPosition(0.0f,-cameraSpeed*deltaSec,0.0f);
    }

    if (game->wInput->IsKeyDown(Keys::D1))
    {
       index=1;        
    }
    if (game->wInput->IsKeyDown(Keys::D2))
    {
        index=2;        
    }
    if (game->wInput->IsKeyDown(Keys::D3))
    {
        index=3;        
    }
    if (game->wInput->IsKeyDown(Keys::D4))
    {
        index=4;        
    }
    if (game->wInput->IsKeyDown(Keys::D5))
    {
        index=5;        
    }
    if (game->wInput->IsKeyDown(Keys::D6))
    {
        index=6;        
    }
    if (game->wInput->IsKeyDown(Keys::D7))
    {
        index=7;        
    }
    if (game->wInput->IsKeyDown(Keys::D8))
    {
        index=8;        
    }
    if (game->wInput->IsKeyDown(Keys::D9))
    {
        index=9;        
    }
    if (game->wInput->IsKeyDown(Keys::D0))
    {
        index=0;        
    }

    if (index!=-1)
    {
        XMVECTOR * determinant;
        this->camera->SetPosition(0,0,0);
        this->camera->SetRotation(0,0,0);
        //this->camera->SetLookAtPos(XMFLOAT3(0,0,0));
        Planet* planet =dynamic_cast<Planet*>(game->gameComponents[index].get());

        
        
        this->camera->AddjustTransformation(XMMatrixInverse(nullptr, planet->rotationOrbitMat));
        this->camera->AddjustTransformation(XMMatrixInverse(nullptr, XMMatrixTranslation(planet->orbitR,0.0f,0)));
        this->camera->AddjustTransformation(XMMatrixInverse(nullptr,planet->rotationAxisMat));
        //this->camera->AddjustTransformation(XMMatrixInverse(nullptr,XMMatrixRotationY(planet->axeAngle)));
        
        
        this->camera->AddjustTransformation(XMMatrixTranslation(planet->r/2.0f+0.50f,0.0f,0));
        //this->camera->AddjustTransformation(XMMatrixRotationY(planet->axeAngle));
        //this->camera->AddjustTransformation(planet->rotationAxisMat);
       
        this->camera->AddjustTransformation(XMMatrixInverse(nullptr,XMMatrixRotationY(M_PI/2.0f)));
    }
        
    
}
