# name of your application
APPLICATION = practice2

# If no BOARD is found in the environment, use this default:
BOARD ?= unwd-range-l1-r3
# BOARD ?= nucleo-f401re


# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

FEATURES_REQUIRED += periph_gpio_irq

# USEMODULE +=xtimer
USEMODULE +=st95
USEMODULE +=lptimer

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include