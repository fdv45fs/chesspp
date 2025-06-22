# ChessDemoAI

Đây là một dự án cờ vua AI đơn giản sử dụng C++.

## Yêu cầu

Trước khi bắt đầu, bạn cần cài đặt các công cụ sau:

1.  **CMake**: Một công cụ để tự động hóa việc xây dựng (build) phần mềm. Bạn có thể tải về tại [cmake.org](https://cmake.org/download/).
2.  **MinGW-w64**: Một môi trường compiler C++ cho Windows. Bạn có thể cài đặt thông qua [MSYS2](https://www.msys2.org/) hoặc tải trực tiếp trình cài đặt từ [trang phát hành của MinGW-w64](https://github.com/niXman/mingw-builds-binaries/releases). Hãy chắc chắn rằng bạn đã thêm đường dẫn của `mingw/bin` vào biến môi trường `PATH` của hệ thống.

## Hướng dẫn biên dịch

Để biên dịch dự án, hãy làm theo các bước sau:

1.  **Mở terminal** (ví dụ: Command Prompt, PowerShell, hoặc Git Bash).

2.  **Tạo một thư mục build** và di chuyển vào đó:
    ```bash
    mkdir build
    cd build
    ```

3.  **Chạy CMake** để tạo các file build. Sử dụng generator "MinGW Makefiles":
    ```bash
    cmake .. -G "MinGW Makefiles"
    ```
    Nếu bạn đã thêm MinGW vào `PATH`, CMake sẽ tự động tìm thấy nó.

4.  **Biên dịch dự án**:
    ```bash
    cmake --build .
    ```
    Hoặc bạn có thể dùng lệnh `mingw32-make`.

5.  **Chạy ứng dụng**:
    Sau khi biên dịch thành công, file thực thi `chess_ai_demo.exe` sẽ được tạo trong thư mục `build`. Bạn có thể chạy nó bằng lệnh:
    ```bash
    ./chess_ai_demo.exe
    ``` 
6. **Sử dụng UI**:
    Chọn chức năng giao diện đồng thời chạy theo dự án này: [UI](https://github.com/MinhHoang137/chessAI_UI_build)