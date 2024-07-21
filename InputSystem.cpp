#include "InputSystem.h"
#include <Windows.h>


InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	POINT current_mouse_pos= {};
	::GetCursorPos(&current_mouse_pos); //get current mouse position
	
	if(m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x,current_mouse_pos.y);
		m_first_time = false;
	}

	if(current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x - m_old_mouse_pos.m_x, current_mouse_pos.y - m_old_mouse_pos.m_y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);


    if (::GetKeyboardState(m_keys_state))
    {
        for (unsigned int i=0;i<256;i++)
		{
			//key is down
			if (m_keys_state[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if(m_keys_state[i] != m_old_keys_state[i]) (*it)->onLeftMouseDown(Point(current_mouse_pos.x,current_mouse_pos.y));
					}
					else if (i == VK_LBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i]) (*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
						(*it)->onKeyDown(i);

					
					++it;
				}
			}
			//key is up
			else
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						if (i== VK_LBUTTON)
						{
							(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON)
						{
							(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
							(*it)->onKeyUp(i);
						++it;
					}
				}
			}
		}
		//store current key state to old key state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char)*256);
    }
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
    std::unordered_set<InputListener*, InputListener*>::iterator it = m_set_listeners.find(listener);
    if (it != m_set_listeners.end())
	{
		m_set_listeners.erase(it);
	}
}

InputSystem* InputSystem::get()
{
    static InputSystem system;
    return &system;
}
