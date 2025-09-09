#include "UIManager.h"
#include "Engine.h"



BaseUI::BaseUI(){
    pUImanager = &Engine::uiManager;
}


BaseUI::~BaseUI() {
    pUImanager->UnregisterElement(this);
};


int UIManager::Init() {}
    int UIManager::Update(uint64_t EuS, uint64_t TuS){

        while(Engine::touchEvents.GetElemNum()){
            InputEvent event;
            Engine::touchEvents.PopElem(event);

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
        
    }
    int UIManager::Destroy() {}
    
    int UIManager::RegisterElement(BaseUI* elem){
        auto pos = std::find(vElements.begin(), vElements.end(), [&elem] (const BaseUI*& A) { return A->fZValue < elem->fZValue; });
        vElements.insert(pos, elem);
    };
    int UIManager::UnregisterElement(BaseUI* elem){
        auto pos = std::find(vElements.begin(), vElements.end(), [&elem] (const BaseUI*& A) { return A == elem; });
        if(pos != vElements.end()){
            vElements.erase(pos);
        }
    }