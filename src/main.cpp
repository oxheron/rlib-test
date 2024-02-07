// lib
#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "renderer/core/window.h"
#include "renderer/definitions.h"
#include "renderer/global.h"
#include "renderer/world/camera.h"
#include "renderer/model/mesh.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <bx/math.h>
#include <renderer/renderpass/renderpass.h>
#include <renderer/texture/texture.h>
#include <renderer/core/shader.h>
#include <engine/framebuffer/framebuffer.h>
#include <engine/draw/drawutils.h>

int main(void)
{
    std::ofstream log_file("log.txt", std::ios_base::app);
    EngineInit init; 
    
    // global->mouse.set_mode(GLFW_CURSOR_DISABLED);
    TextureAtlas atlas(2048, 2048, 5);
    BatchManager manager(pos_tex_norm(), model_tex_instance(), "fill_compute.cs");

    auto program = load_program("batch_vertex.vs", "batch_fragment.fs");

    StandardModel mesh;
    mesh.load_mesh("resources/models/cube.json");
    mesh.load_texture(&atlas);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 2.0, 1.0));
    // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
    mesh.set_modelmat(model);
    mesh.upload(&manager); 

    // MRT clear
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.f, 0, 0);
    bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0, 1.f, 0);

    Camera camera;
    camera.add_pitch(35.264);
    camera.add_yaw(45);

    bgfx::TextureHandle nmbuffer[2];
    bgfx::Attachment nmbuffer_attachment[2];

    bgfx::TextureFormat::Enum depthFormat = bgfx::isTextureValid(0, false, 1, 
        bgfx::TextureFormat::D32F, BGFX_TEXTURE_RT) ? 
        bgfx::TextureFormat::D32F : bgfx::TextureFormat::D24;

    nmbuffer[1] = bgfx::createTexture2D(window_width, window_height, false, 1, 
        depthFormat, BGFX_TEXTURE_RT);

    nmbuffer[0] = bgfx::createTexture2D(window_width, window_height, false, 1, 
        bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);

    nmbuffer_attachment[0].init(nmbuffer[0]);
    nmbuffer_attachment[1].init(nmbuffer[1]);

    auto framebuffer = bgfx::createFrameBuffer(2, nmbuffer_attachment);

    auto framebuffer_sampler = bgfx::createUniform("framebuffer", 
        bgfx::UniformType::Sampler);

    auto width_height = bgfx::createUniform("width_height", 
        bgfx::UniformType::Vec4);

    auto point = bgfx::createUniform("light", 
        bgfx::UniformType::Vec4);

    auto inv_mat = bgfx::createUniform("inv_viewproj", 
        bgfx::UniformType::Mat4);

    auto blit_program = load_program("fsq.vs", "blitfsq.fs");

    std::cout << bgfx::getRendererName(bgfx::getRendererType()) << std::endl;

    while (!window_should_close())
    {
        if (global->kb[GLFW_KEY_W].held) camera.movey(-0.0000001 * global->tm.get_delta_time().count());
        if (global->kb[GLFW_KEY_S].held) camera.movey(0.0000001 * global->tm.get_delta_time().count());
        if (global->kb[GLFW_KEY_A].held) camera.movex(0.0000001 * global->tm.get_delta_time().count());
        if (global->kb[GLFW_KEY_D].held) camera.movex(-0.0000001 * global->tm.get_delta_time().count());
        if (global->kb[GLFW_KEY_ESCAPE].get_pressed()) break;
        
        window_update();
        global->tm.update();
        global->mouse.poll_mouse();
        
        global->bgfx->update();

        glm::mat4 orthomat = glm::orthoLH_ZO<float>(-window_width/80, window_width/80, -window_height/80, window_height/80, -100, 100);
        // glm::mat4 orthomat = glm::perspectiveLH_ZO<float>(120, window_width/window_height, 0.1, 100);
        glm::mat4 view = camera.get_view();

        bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(orthomat));
        bgfx::setViewRect(0, 0, 0, window_width, window_height);

        // bgfx::Encoder* encoder = bgfx::begin();
        // atlas.bind(encoder);
        // manager.draw(0, program, encoder);
        // bgfx::end(encoder);

        bgfx::setViewFrameBuffer(0, framebuffer);

        auto encoder = bgfx::begin();

        atlas.bind(encoder);
        manager.draw(0, program, encoder);

        render_framebuffer(framebuffer, framebuffer_sampler, 1, 1, encoder); 
        bgfx::setViewRect(1, 0, 0, window_width, window_height);

        float wh_data[4] = {(float) window_width, (float) window_height, 0, 0};

        encoder->setUniform(width_height, wh_data);

        glm::mat4 inv_viewproj = view * orthomat;
        inv_viewproj = glm::inverse(inv_viewproj);

        float point_data[4] = {0, 0, 0, 1};

        encoder->setUniform(inv_mat, glm::value_ptr(inv_viewproj));
        encoder->setUniform(point, point_data);
        bgfx::setViewTransform(1, glm::value_ptr(view), 
            glm::value_ptr(orthomat));

        encoder->submit(1, blit_program);

        bgfx::end(encoder);

        bgfx::frame();

        // global->tm.hold_at_fps();
        if (global->tm.is_second()) std::cout << global->tm.get_fps() << std::endl;
    }
}

