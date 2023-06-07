#include "config.h"
#include <argumentum/argparse-h.h>

namespace core {
using Error = Config::Error;
cpp::result<Config, Error> Config::create_from_args(int argc, char** argv) {
  Config config{};
  auto args = argumentum::argument_parser();
  auto params = args.params();

  auto& resolution = config.m_resolution;
  params.add_parameter(resolution.width, "-w", "--width")
          .required(false)
          .default_value(DEFAULT_SCREEN_WIDTH)
          .maxargs(1);
  params.add_parameter(resolution.height, "-h", "--height")
          .required(false)
          .default_value(DEFAULT_SCREEN_HEIGHT)
          .maxargs(1);
  params.add_parameter(resolution.target_fps, "-f", "--target-fps")
          .required(false)
          .default_value(DEFAULT_TARGET_FPS)
          .maxargs(1);
  args.add_default_help_option();
  auto result = args.parse_args(argc, argv);

  if (result)
    return config;

  if (result.help_was_shown())
    return cpp::fail(Error::HelpShown);

  if (result.errors_were_shown())
    return cpp::fail(Error::InvalidArguments);

  return cpp::fail(Error::Unknown);
}
}// namespace core
