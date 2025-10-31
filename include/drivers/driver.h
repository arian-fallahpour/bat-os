#ifndef __BATOS__DRIVERS__DRIVER_H
#define __BATOS__DRIVERS__DRIVER_H

namespace batos {
    namespace drivers {
        class Driver {
            public:    
                Driver();
                ~Driver();

                virtual void Activate();
                virtual int Reset();
                virtual void Deactivate();
        };

        class DriverManager {
            private:
                batos::drivers::Driver* drivers[256];
                int numDrivers;
            public:
                DriverManager();
                void addDriver(batos::drivers::Driver*);

                void ActivateAll();
        };
    }
};





#endif