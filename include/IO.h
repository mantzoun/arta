#ifndef __IO__H
#define __IO__H

#include<thread>

#include "include/Logger.h"
#include "include/MessageConsumer.h"

namespace arta {
    /**
     * @class IO
     *
     * @brief Class IO handles the local input/output for
     *        the running process
     *
     *        Currently only oneway reading from a named fifo
     *        is implemented
     */
    class IO
    {
    private:
        bool        fifoStop;
        std::thread * fifoThread = NULL;
        std::string fifoPath;
        void        * fifoCallback;
        Logger * logger = NULL;
        MessageConsumer * consumer;

       /**
        * fifoRead
        *
        * read from the defined named fifo
        */
        void fifoRead(void);

    public:
        /**
         * @brief Default constructor
         */
        IO(void);

        /**
         * @brief Initialize the named fifo
         *
         *        Start the thread that will parse input from the fifo
         *
         * @param fifo The path of the fifo
         *
         * @return true if in case os success
         */
        bool fifoInit(std::string fifo);

        /**
         * @brief Terminate the fifo
         *
         *        stop the fifoRead thread
         *
         * @return true if the thread was running and was terminated, false otherwise
         */
        bool fifoTerminate(void);

        /**
         * @brief Set the logger object
         *
         * @param logger The logger object
         */
        void setLogger(Logger * logger);

        /**
         * @brief Set a pointer to the consumer module
         *
         *        The consumer is able to receive messages from the
         *        IO.
         *
         * @param consumer The consumer object
         */
        void setConsumer(MessageConsumer * consumer);
    };
}

#endif /* __IO__H */
