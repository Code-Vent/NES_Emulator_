#include<stdio.h>
#include"nes.h"
#include"raylib/raylib.h"
#include"raylib/rlgl.h"
#include"GLFW/glfw3.h"

int main(int argc, char* argv[]) {
    
    struct Nes nes;
    setup(&nes);
    
    write(nes.main_bus, 0xfffc, 0x00);
    write(nes.main_bus, 0xfffd, 0x80);
    write(nes.main_bus, 0x8000, 0xa9);
    write(nes.main_bus, 0x8001, 0x42);
    write(nes.main_bus, 0x8002, 0x8d);
    write(nes.main_bus, 0x8003, 0x00);
    write(nes.main_bus, 0x8004, 0x02);
    write(nes.main_bus, 0x8005, 0x4c);
    write(nes.main_bus, 0x8006, 0x00);
    write(nes.main_bus, 0x8007, 0x80);

    hardReset(nes.cpu);
    while (execute(nes.cpu) && !nes.main_bus->error) {
        struct Registers r = getRegisters(nes.cpu);
        printf("\nPC = %x\n ", r.pc);
        printf("SP = %x\n ", r.sp);
        printf("A = %x\n ", r.a);
    }
    
    destroy(&nes);
    /*

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    
    // 800x450 is 16:9
    InitWindow(800, 450, "Raylib Custom GLFW Context");
    //SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
    */
    return 0;
}