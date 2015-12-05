package no.nith.pg5100.config;

import no.nith.pg5100.controller.EventController;

import javax.ws.rs.ApplicationPath;
import javax.ws.rs.core.Application;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by Ekaterina Orlova on 05/12/15.
 */
@ApplicationPath("/services")
public class ApplicationConfig extends Application {
    private final Set<Class<?>> classes;

    public ApplicationConfig() {
        classes = new HashSet<>();
        classes.add(EventController.class);
    }

    @Override
    public Set<Class<?>> getClasses() {
        return classes;
    }
}

