// #include "rx-request-manager.hpp"

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <chrono>
#include <thread>
#include "http-client/http-client.hpp"


#include <rxcpp/rx.hpp>

int main()
{
    rxcpp::schedulers::run_loop run_loop;
    auto main_thread = rxcpp::observe_on_run_loop(run_loop);
    auto worker_thread = rxcpp::observe_on_event_loop();
    rxcpp::composite_subscription lifetime;
    rxcpp::composite_subscription lifetime2;


    const auto get_url = "https://httpbin.org/get";
    const auto post_url = "https://httpbin.org/post";

    HttpClient client;
    
    rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
   {
        try
        {
            client.build()->Get(get_url)
            .add_curl_option(CURLOPT_VERBOSE, 0L)
            .process_response([&](const HttpResponse& result)
            {
                if (result.is_ok())
                {
                    try
                    {
                        out.on_next(result);
                        out.on_completed();
                    } catch ( ... ) {
                        out.on_error(std::current_exception());
                    }
                } else {
                    out.on_next(result);
                    out.on_completed();
                }
            })
            .send_request();
        } catch ( ... ) {
            out.on_error(std::current_exception());
        }
    }).subscribe_on(worker_thread)
    .observe_on(main_thread)
    .subscribe
    (lifetime,
        [&](const HttpResponse& r)
        {
            std::cout << r.body << std::endl;
        },
        [](std::exception_ptr ep)
        {
            std::cout << "on error" << std::endl;
        }, [] ()
        {
            std::cout << "on completed!" << std::endl;
        }
    );

    rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            client.build()->Post(post_url)
            .accept_json()
            .add_json_body("")
            .add_header("Cache-Control: no-cache, no-store, must-revalidate")
            .add_header("Pragma: no-cache")
            .add_header("Expires: 0")
            .process_response([&](const HttpResponse& result)
            {
                if (result.is_ok())
                {
                    try
                    {
                        out.on_next(result);
                        out.on_completed();
                    } catch ( ... ) {
                        out.on_error(std::current_exception());
                    }
                    
                } else {
                    out.on_next(result);
                    out.on_completed();
                }
            })
            .send_request();
        } catch ( ... ) {
            out.on_error(std::current_exception());
        }
    }).subscribe_on(worker_thread)
    .observe_on(main_thread)
    .subscribe
    (lifetime2,
        [&](const HttpResponse& r)
        {
            std::cout << r.body << std::endl;
        },
        [](std::exception_ptr ep)
        {
            std::cout << "on error" << std::endl;
        }, [] ()
        {
            std::cout << "on completed!" << std::endl;
        }
    );

    while (lifetime.is_subscribed() || lifetime2.is_subscribed() || !run_loop.empty())
    {
        while (!run_loop.empty() && run_loop.peek().when < run_loop.now())
        {
            run_loop.dispatch();
        }
    }
    return 0;
}



































/* Garbage section */

// client.build()->Get(get_url)
    // .add_curl_option(CURLOPT_VERBOSE, 0L)
    // .process_response([&](const HttpResponse& result)
    // {
    //     if (result.is_ok())
    //     {
    //         try
    //         {
    //             std::cout << "Result Body: " << result.body << std::endl;
    //             std::cout << "Result headers: " << result.header << std::endl;
    //         } catch (const std::exception& ex) {
    //             std::cout << ex.what() << std::endl;
    //             return;
    //         }
            
    //     } else {
    //         std::cout << "Request failed: " << result.error_msg << std::endl
    //             << "HTTP code: " << result.http_response_code << std::endl;
    //     }
    // })
    // .send_request();

    // client.build()->Post(post_url)
    //     .accept_json()
    //     .add_json_body("")
    //     .add_header("Cache-Control: no-cache, no-store, must-revalidate")
    //     .add_header("Pragma: no-cache")
    //     .add_header("Expires: 0")
    //     .process_response([&](const HttpResponse& result)
    //     {
    //         if (result.is_ok())
    //         {
    //             try
    //             {
    //                 std::cout << "Result Body: " << result.body << std::endl;
    //                 std::cout << "Result headers: " << result.header << std::endl;
    //             } catch (const std::exception& ex) {
    //                 std::cout << ex.what() << std::endl;
    //                 return;
    //             }
                
    //         } else {
    //             std::cout << "Request failed: " << result.error_msg << std::endl
    //                 << "HTTP code: " << result.http_response_code << std::endl;
    //         }
    //     })
    //     .send_request();

        // auto values = rxcpp::observable<>::range(1);

    // auto s1 = values.subscribe_on(threads).map([](int prime)
    //     {
    //         CTDetatils();
    //         Yield(true);
    //         return std::make_tuple("1:",prime);
    //     }
    // );

    // auto s2 = values.subscribe_on(threads).map([](int prime)
    //     {
    //         CTDetatils();
    //         Yield(true);
    //         return std::make_tuple("2:",prime);
    //     }
    // );

    // s1.merge(s2).take(100).as_blocking().subscribe(rxcpp::util::apply_to(
    //     [](const char* s, int p) {
    //         CTDetatils();
    //         console_mutex.lock();
    //         printf("%s %d\n",s,p);
    //         console_mutex.unlock();
    //     }
    // ));

