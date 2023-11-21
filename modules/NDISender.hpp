#ifndef _NDI_SENDER_HPP
#define _NDI_SENDER_HPP

#include <WidgetOp.hpp>

class NDISender : WidgetOp
{
public:
    void update();
    void doGui() override;

    ~NDISender() override;

    void captureFramrbufferAndAssignToNDI();

    void sendToNDI();

private:
    void initializeNDI();
    // NDI variables
    NDIlib_send_instance_t pNDI_send;
    NDIlib_video_frame_v2_t video_frame;
}
#endif