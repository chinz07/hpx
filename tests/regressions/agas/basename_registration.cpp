//  Copyright (c) 2015 Andreas Schaefer
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/lcos/broadcast.hpp>
#include <hpx/lcos/local/receive_buffer.hpp>
#include <hpx/lcos/barrier.hpp>
#include <hpx/runtime/get_ptr.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>

static std::string itoa(int i)
{
    std::stringstream buf;
    buf << i;
    return buf.str();
}

struct test_server
  : hpx::components::simple_component_base<test_server>
{
    test_server()
    {}

    hpx::id_type call() const
    {
        return hpx::find_here();
    }
    HPX_DEFINE_COMPONENT_ACTION(test_server, call, call_action);
};

typedef hpx::components::simple_component<test_server> server_type;
HPX_REGISTER_COMPONENT(server_type, test_server);

typedef test_server::call_action call_action;
HPX_REGISTER_ACTION(call_action);

std::string genName(int source)
{
    return "/0/" +
        itoa(source);
}

void testBar()
{
    int rank = hpx::get_locality_id();
    int num_ranks = hpx::get_num_localities().get();

    std::vector<hpx::id_type> receivers;
    std::vector<hpx::id_type> accepters;
    for (int i = 0; i < num_ranks; ++i) {
        if (i == rank)
            continue;

        std::string name = genName(i);
        std::cout << "registration: " << name << "/" << rank << "\n";

        hpx::id_type id = hpx::new_<test_server>(hpx::find_here()).get();

        hpx::register_with_basename(name, id, rank).get();
        std::cout << "registration done: " << name << "/" << rank << "\n";
        receivers.push_back(id);
    }

    for (int i = 0; i < num_ranks; ++i) {
        if (i == rank)
            continue;

        std::string name = genName(rank);
        std::cout << "lookup: " << name << "/" << i << "\n";
        hpx::future<hpx::id_type> id = hpx::find_from_basename(name, i);
        accepters.push_back(std::move(id.get()));
        std::cout << "lookup done: " << name << "/" << i << "\n";
    }

    std::cout << "all done " << rank << "\n";
}

int hpx_main(int argc, char **argv)
{
    testBar();

    return hpx::finalize();
}

int main(int argc, char **argv)
{
    // We want HPX to run hpx_main() on all localities to avoid the
    // initial overhead caused by broadcasting the work from one to
    // all other localities:
    std::vector<std::string> config(1, "hpx.run_hpx_main!=1");

    return hpx::init(argc, argv, config);
}
