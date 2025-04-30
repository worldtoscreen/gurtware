#include "../include/lazy.h"

typedef BOOL( *SB )( HDC );
SB o__wglSwapBuffers = nullptr;

bool GetWindowDimensions( __int32 &WindowWidth, __int32 &WindowHeight )
{
    if ( !instances::executable )
        return false;

    WindowWidth = *(__int32 *)( instances::executable + offsets::WindowWidth );
    WindowHeight = *(__int32 *)( instances::executable + offsets::WindowHeight );
    return true;
}

BOOL hookedWglSwapBuffers( HDC DeviceContext )
{
    if ( globals::uninjecting ) return o__wglSwapBuffers( DeviceContext );
    if ( !globals::imgui_init )
    {
        IMGUI_CHECKVERSION( );
        ImGui::CreateContext( );
        ImGuiIO &io = ImGui::GetIO( ); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsClassic();

        ImGui_ImplWin32_InitForOpenGL( globals::game_hwnd );
        ImGui_ImplOpenGL3_Init( );


        globals::imgui_init = true;
        printf( "[+] initalized imgui\n\n" );
        return o__wglSwapBuffers( DeviceContext );
    }

    ImGui::GetIO( ).WantCaptureKeyboard = true;
    ImGui::GetIO( ).WantCaptureMouse = true;
    ImGui::GetIO( ).MouseDrawCursor = true;
    ImGui::GetIO( ).MouseDown[0] = GetAsyncKeyState( VK_LBUTTON );
    ImGui::GetIO( ).MouseDown[1] = GetAsyncKeyState( VK_RBUTTON );

    ImGui_ImplOpenGL3_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );

    if ( globals::menu_open )
    {
        ImGui::Begin( "gurtware" );
        ImGui::Text( "gurt: yo\nengine %llX\nexecutable %llX\ncgame %llX\ngame %llX\nwglSwapBuffers %llX", 
            instances::engine,
            instances::executable,
            instances::game,
            instances::cgame,
            (uintptr_t)o__wglSwapBuffers
        );
        ImGui::End( );
    }

    ImGui::Render( );

    __int32 g_Width, g_Height;
    if ( GetWindowDimensions( g_Width, g_Height ) )
    {
        glViewport( 0, 0, g_Width, g_Height );
    }
    
    // glClearColor( 0.f, 0.f, 0.f, 0.f );
    // glClear( GL_COLOR_BUFFER_BIT );
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

    return o__wglSwapBuffers( DeviceContext );
}


void MainThread( HMODULE LParam )
{
    AllocConsole( );
    FILE *Dummy;
    freopen_s( &Dummy, "CONOUT$", "w", stdout );
    freopen_s( &Dummy, "CONIN$", "r", stdin );

    MH_Initialize( );

    if ( !instances::init() )
    {
        printf( "[-] failed to read module list" );
        if ( Dummy ) fclose( Dummy );
        FreeConsole( );

        FreeLibraryAndExitThread( LParam, 0 );
    }

    globals::game_hwnd = *(HWND *)( instances::engine + offsets::hwnd );
    if ( !globals::game_hwnd )
    {
        printf( "[-] failed to read window hwnd" );
        if ( Dummy ) fclose( Dummy );
        FreeConsole( );

        FreeLibraryAndExitThread( LParam, 0 );
        return;
    }
    printf( "[+] Read GameBorder + GameHWND (%llX)\n", (long long)globals::game_hwnd );

    HMODULE hOpenGL = GetModuleHandleA( "opengl32.dll" );
    SB wglSwapBuffers = nullptr;

    if ( hOpenGL )
    {
        wglSwapBuffers = (SB)(uintptr_t)GetProcAddress( hOpenGL, "wglSwapBuffers" );
        printf( "[+] opengl32.dll!wglSwapBuffers %llX\n", (uintptr_t)wglSwapBuffers );
        MH_CreateHook( reinterpret_cast<LPVOID>( wglSwapBuffers ), &hookedWglSwapBuffers, reinterpret_cast<LPVOID *>( &o__wglSwapBuffers ) );
        printf( "[+] hooked wglSwapBuffers -> %d\n", MH_EnableHook( reinterpret_cast<LPVOID>( wglSwapBuffers ) ) );
    }

    printf( "[+] init finished, waiting for F6.\n" );
    while ( !GetAsyncKeyState( VK_F6 ) )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        
        if ( GetAsyncKeyState( VK_INSERT ) & 1 ) globals::menu_open = !globals::menu_open;
    }

    printf( "[+] starting uninject\n" );
    globals::uninjecting = true;

    printf( "[+] shutting down imgui\n" );
    ImGui_ImplOpenGL3_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    if ( wglSwapBuffers )
    {
        printf( "[+] unhooking wglSwapBuffers\n" );
        MH_DisableHook( (LPVOID)wglSwapBuffers );
    }

    MH_Uninitialize( );

    printf( "[+] done\n" );
    if ( Dummy ) fclose( Dummy );
    FreeConsole( );
    
    FreeLibraryAndExitThread( LParam, 0 );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD reason, LPVOID lpReserved )
{
    switch ( reason )
    {
    case DLL_PROCESS_ATTACH:
        CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0 );
        break;
    }

    return TRUE;
}