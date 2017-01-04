/* Author: Pouya Mohammadi
 * Date:   09/06/2016
 *
 * Description: This is a simple orocos/rtt component template. It should be
 *              modified and extended by users to accomodate their needs.
 */

#ifndef OROCOSJOYSTICKCOMPONENT_HPP
#define OROCOSJOYSTICKCOMPONENT_HPP

#include <rtt/RTT.hpp>
#include <rtt/Port.hpp>
#include <string>

#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>

class JoystickComponent: public RTT::TaskContext {
public:
    JoystickComponent(std::string const & name);

    // RTT::TaskContext methods that are needed for any standard component and
    // should be implemented by user
    bool configureHook();
    bool startHook();
    void updateHook();
    void stopHook();
    void cleanupHook();

private:
    // Declare ports and their datatypes
    int joy_file_handle;
    struct js_event joy_event;

    // ports
    std::vector<int> joy_buttons, joy_axes;
    RTT::OutputPort<std::vector<int> > joy_buttons_port, joy_axes_port;

    int ax_count, bt_count;
};

#endif // OROCOSJOYSTICKCOMPONENT_HPP
