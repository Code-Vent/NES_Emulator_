#include<stdio.h>
#include"nes.h"
#include"raylib/raylib.h"
//#include"raylib/rlgl.h"
#include"GLFW/glfw3.h"
#include"ppu2C02.h"

void initRaylib() {
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    // 800x450 is 16:9
    InitWindow(800, 450, "Raylib Custom GLFW Context");
    //SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
}

int main(int argc, char* argv[]) {
    //initRaylib();
    struct Nes nes;
    setup(&nes);
    
    write(nes.main_bus, 0xfffc, 0x00);
    write(nes.main_bus, 0xfffd, 0x00);
    write(nes.main_bus, 0x0000, 0xa9);
    write(nes.main_bus, 0x0001, 0x42);
    write(nes.main_bus, 0x0002, 0x8d);
    write(nes.main_bus, 0x0003, 0x00);
    write(nes.main_bus, 0x0004, 0x02);
    write(nes.main_bus, 0x0005, 0x4c);
    write(nes.main_bus, 0x0006, 0x00);
    write(nes.main_bus, 0x0007, 0x00);

    hardReset(nes.cpu);
    struct Registers r;
    
    while (execute(nes.cpu) && !nes.main_bus->error) {
        getCPURegisters(&r);
        printf("\nPC = %x\n ", r.pc);
        printf("SP = %x\n ", r.sp);
        printf("A = %x\n ", r.a);
    }
    
    destroy(&nes);

    
    
    return 0;
}

