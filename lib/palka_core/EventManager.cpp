//
// Created by NePutin on 8/21/2021.
//

#include "EventManager.h"

using namespace palka;

std::multimap<KBoardEvent, std::function<void(EventData&)>>  EventManager::KeyboardEvents{};
std::multimap<KBoardEvent, std::function<void()>>  EventManager::KeyboardInputs{};
std::multimap<MouseEvent, std::function<void(EventData&)>>  EventManager::MouseEvents{};
std::multimap<EventType, std::function<void(EventData&)>>  EventManager::TypeEvents{};
std::set<int>  EventManager::keyPressed{};
std::set<MouseEvent::Mouse_Button>  EventManager::mousebPress{};