#include "GroupSelector.h"
#include "Debug.h"

static GroupSelector& GroupSelector::get_instance(uint8_t plus_pin, uint8_t minus_pin)
{
  static GroupSelector selector(plus_pin, minus_pin, 
                                [](){selector.isr_plus_button();},
                                [](){selector.isr_minus_button();});
  return selector;
}

GroupSelector::GroupSelector(uint8_t plus_pin, uint8_t minus_pin,
                              ButtonCB plus_cb, ButtonCB minus_cb):
                              plus(plus_pin, plus_cb), minus(minus_pin, minus_cb)
{
  enabled = false;
  current_group = FIRST_GROUP;
}

void GroupSelector::disable(void)
{
  enabled = false;
  plus.disable();
  minus.disable();
}

void GroupSelector::enable(void)
{
  enabled = true;
  plus_pressed = false;
  minus_pressed = false;
  plus.enable();
  minus.enable();
}

void GroupSelector::update(void)
{
  changed = false;

  if(enabled)
  {
    disable();

    if(plus_pressed)
    {
      if(current_group == NUM_GROUPS)
      {
        current_group = FIRST_GROUP;
      }
      else
      {
        current_group++;
      }
      changed = !changed;
    }

    if(minus_pressed)
    {
      if(current_group == FIRST_GROUP)
      {
        current_group = NUM_GROUPS;
      }
      else
      {
        current_group--;
      }
      changed = !changed;
    }
    enable();
  }
}

static void GroupSelector::isr_plus_button(void)
{
  Debug.println("Plus");
  plus_pressed = true;
}

static void GroupSelector::isr_minus_button(void)
{
  Debug.println("Minus");
  minus_pressed = true;
}