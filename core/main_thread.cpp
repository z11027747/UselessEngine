
// #include <iostream>
// #include <thread>
// #include <windows.h>
// #include <queue>
// #include <mutex>

// std::queue<int> msgQue;
// std::mutex msgQue_mutex;

// static void enqueue_msg(int &number)
// {
//     std::lock_guard<std::mutex> lg(msgQue_mutex);
//     msgQue.push(number++);
// }

// static void dequeue_all()
// {
//     std::lock_guard<std::mutex> lg(msgQue_mutex);
//     while (!msgQue.empty())
//     {
//         int result = msgQue.front();
//         msgQue.pop();
//         std::cout << result << "\n";
//     }
// }

// static int num = 0;

// class Logic
// {
// public:
//     static void Update()
//     {
//         while (true)
//         {
//             std::cout << "Logic Update  \n";
//             enqueue_msg(num);
//             Sleep(1000);
//         }
//     }
// };
// class Render
// {
// public:
//     static void Update()
//     {
//         while (true)
//         {
//             std::cout << "Render Update  \n";
//             dequeue_all();
//             Sleep(1000);
//         }
//     }
// };

// int main()
// {
//     std::cout << "Hello World!\n";

//     Logic logic;
//     Render render;

//     std::thread logicThread(&Logic::Update);
//     std::thread renderThread(&Render::Update);

//     while (true)
//     {
//     }
// }
