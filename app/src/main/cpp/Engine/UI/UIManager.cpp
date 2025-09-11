#include "UIManager.h"
#include "Engine/Main/Engine.h"
#include "Engine/UI/Types/Keyboard.h"
#include "World/World.h"



BaseUI::BaseUI(){
    pUImanager = Engine::uiManager.get();
}


BaseUI::~BaseUI() {
    pUImanager->UnregisterElement(this);
};

UIManager::UIManager() {
    pKeyboard = std::make_shared<KeyBoard>();

}


int UIManager::Init() { 
    LOGI("Init UIManager");
    if(pKeyboard){
        Engine::world->AddObject(pKeyboard);
    }
    
    
    return 0;
}



    int UIManager::Update(uint64_t EuS, uint64_t TuS){

        while(Engine::touchEvents->GetElemNum()){
            InputEvent event;
            Engine::touchEvents->PopElem(event);

            //Forward the elements
            for(int i = 0; i < vElements.size(); --i){
                if(vElements[i]->bIsActive){
                    //If the element acts upon the data
                    if(vElements[i]->UIEvent(event)){
                        //Check if it consumes the event
                        if(vElements[i]->bConsumeInput){
                            //Break loop
                            break;
                        }
                    }
                }
            }
        //Get next event
        }
        
        return 0;
    }
    int UIManager::Destroy() { return 0;}
    
    int UIManager::RegisterElement(BaseUI* elem){
        if(!elem) {
            LOGE_FL("Element is nullptr");
            return 1;
        }

        LOGIVV("Test SOmething");
        vElements = std::vector<BaseUI*>();

        LOGIVV("Test Register");
        LOGIVVV("Size Elements: " + std::to_string(vElements.size()));
        //Check if elem already exists
        for(BaseUI*& e : vElements){
            LOGIVVV("Test");
            if(e == elem){
                LOGIVV("Element already registered");
                return 0;
            }
            if(!e){
                LOGE_FL("Element in elements list is nullptr");
                return 1;
            }
        }
        LOGIVVV("Test Register 1");
        auto pos = std::find_if(vElements.begin(), vElements.end(), [elem] (BaseUI* A) { return A->fZValue < elem->fZValue; });
        LOGIVVV("Test Register 2");
        vElements.insert(pos, elem);
        LOGIVVV("Test Register 3");
        return 0;
    }
    int UIManager::UnregisterElement(BaseUI* elem){
        auto pos = std::find_if(vElements.begin(), vElements.end(), [&elem] (BaseUI* A) { return A == elem; });
        if(pos != vElements.end()){
            vElements.erase(pos);
        }
        return 0;
    }