package no.nith.pg5100.infrastructure.event;

import no.nith.pg5100.dto.Event;

import java.util.List;

public interface EventDao {
    Event persist(Event event);
    Event findById(int id);
    List<Event> getAll();
}
