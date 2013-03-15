#include <rtthread.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/rtgui_app.h>
#include "appmgr.h"
#include "statusbar.h"

void init_entry(void *param)
{
    rt_device_t device;
    struct rt_device_rect_info info;
#ifndef _WIN32
    rt_device_t device;
	/* init driver */
	rt_hw_sdl_start();
	device = rt_device_find("sdl");

    if (device != RT_NULL)
    {
        info.width = 800;
        info.height = 480;
        /* set graphic resolution */
        rt_device_control(device, RTGRAPHIC_CTRL_SET_MODE, &info);
    }

    rtgui_graphic_set_device(device);

    rtgui_system_server_init();

    /*font system init*/
    rtgui_font_system_init();
#endif
    app_mgr_init();

#ifndef _WIN32
    calibration_init();
#endif
}

void rt_application_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
                           init_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX / 4, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

