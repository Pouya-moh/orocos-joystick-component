/* Author: Pouya Mohammadi
 * Date:   09/06/2016
 *
 * Description: This is a simple orocos/rtt component template. It should be
 *              modified and extended by users to accomodate their needs.
 */

#include "JoystickComponent.hpp"
// needed for the macro at the end of this file:
#include <rtt/Component.hpp>

JoystickComponent::JoystickComponent(std::string const & name) : RTT::TaskContext(name) {
    // constructor:
    ax_count = 0;
    bt_count = 0;
}

bool JoystickComponent::configureHook() {
    // configuring the joystick. TODO: add operation to change /dev/input file
    this->joy_file_handle = open ("/dev/input/js0", O_RDONLY);
    if (joy_file_handle == -1) {
        RTT::log(RTT::Error) << "Joystick not found."<<RTT::endlog();
        return false;
    } else {
        ioctl( joy_file_handle, JSIOCGAXES, &ax_count );
        ioctl( joy_file_handle, JSIOCGBUTTONS, &bt_count );
        RTT::log(RTT::Info) << "Joystick has "<<ax_count<<" axes and "<<bt_count<<" buttons."<<RTT::endlog();
        //setting ports
        joy_axes    = std::vector<int>(ax_count);
        joy_buttons = std::vector<int>(bt_count);
        for (int i=0; i<joy_axes.size(); ++i)    joy_axes.at(i) = 0;
        for (int i=0; i<joy_buttons.size(); ++i) joy_buttons.at(i) = 0;
        joy_axes_port.setName("joy_axes_port");
        joy_buttons_port.setName("joy_buttons_port");
        joy_axes_port.setDataSample(joy_axes);
        joy_buttons_port.setDataSample(joy_buttons);
        ports()->addPort(joy_axes_port);
        ports()->addPort(joy_buttons_port);
        return true;
    }
}

bool JoystickComponent::startHook() {
    // this method starts the component
    return true;
}

void JoystickComponent::updateHook() {
    if (read(joy_file_handle, &joy_event, sizeof(js_event)) == -1 && errno != EAGAIN) {
            RTT::log(RTT::Info) <<"Joystick is closed :("<<RTT::endlog();
    } else {
        if (joy_event.type == JS_EVENT_AXIS) {
            joy_axes.at(int(joy_event.number)) = joy_event.value;
        }
        if (joy_event.type == JS_EVENT_BUTTON) {
            joy_buttons.at(int(joy_event.number)) = joy_event.value;
        }
        joy_axes_port.write(joy_axes);
        joy_buttons_port.write(joy_buttons);
    }
}

void JoystickComponent::stopHook() {
    // stops the component (update hook wont be  called anymore)
    close(this->joy_file_handle);
}

void JoystickComponent::cleanupHook() {
    // cleaning the component data
}

// This macro, as you can see, creates the component. Every component should have this!
ORO_CREATE_COMPONENT_LIBRARY()ORO_LIST_COMPONENT_TYPE(JoystickComponent)
