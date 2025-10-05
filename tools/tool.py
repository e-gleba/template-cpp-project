#!/usr/bin/env python3

import dearpygui.dearpygui as dpg
import logging
import time
import sys


def setup_logging() -> logging.Logger:
    """Setup minimal logging."""
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )
    return logging.getLogger(__name__)


class App:
    def __init__(self):
        self.logger = setup_logging()

    def log_event(self, event: str, **kwargs) -> None:
        """Log application events."""
        if kwargs:
            self.logger.info(f"{event}: {kwargs}")
        else:
            self.logger.info(event)

    def shutdown(self) -> None:
        """Clean shutdown."""
        self.log_event("shutdown", status="initiated")
        dpg.stop_dearpygui()
        time.sleep(0.1)  # Avoid segfault
        self.log_event("shutdown", status="complete")

    def run(self) -> int:
        """Main application loop."""
        self.log_event("startup", status="init")

        try:
            dpg.create_context()
            dpg.create_viewport(title="Modern Dear PyGui", width=400, height=300)

            with dpg.window(label="Main", tag="primary_window"):
                dpg.add_text("Modern, Robust Dear PyGui Example", tag="status_text")
                dpg.add_button(label="Exit", callback=self.shutdown)

            dpg.set_primary_window("primary_window", True)
            dpg.setup_dearpygui()
            dpg.show_viewport()

            self.log_event("startup", status="ready")
            dpg.start_dearpygui()

        except Exception as e:
            self.log_event("error", message=str(e), status="fatal")
            self.logger.exception("Fatal error occurred")
            return 1
        finally:
            self.shutdown()
            self.log_event("shutdown", status="cleanup")

        return 0


def main() -> int:
    """Entry point."""
    app = App()
    return app.run()


if __name__ == "__main__":
    sys.exit(main())