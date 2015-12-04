package no.nith.pg5100.controller;

import no.nith.pg5100.dto.Event;
import no.nith.pg5100.dto.EventType;
import no.nith.pg5100.infrastructure.event.EventDao;
import no.nith.pg5100.infrastructure.subject.SubjectDao;

import javax.annotation.PostConstruct;
import javax.enterprise.inject.Model;
import javax.faces.model.SelectItem;
import javax.inject.Inject;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@Model
public class EventController {
    @Inject
    private EventDao eventDao;

    @Inject
    private SubjectDao subjectDao;

    private Event event;

    private int selectedId;

    private int subjectId;

    public List<Event> getAll() {
        return eventDao.getAll().stream()
                .sorted((e1, e2) ->
                        e2.getStartDateTime() == null ? -1 : e1.getStartDateTime() == null ? 1 : e2.getStartDateTime().compareTo(e1.getStartDateTime()))
                .collect(Collectors.toList());
    }

    @PostConstruct
    public void init() {
        event = new Event();
    }

    public void persist() {
        event.setSubject(subjectDao.findById(subjectId));
        eventDao.persist(event);
    }

    public void initEvent() {
        event = eventDao.findById(selectedId);
    }

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public Event getEvent() {
        return event;
    }

    public void setEvent(Event event) {
        this.event = event;
    }

    public List<SelectItem> getEventTypes() {
        return Stream.of(EventType.values())
                .map(t -> new SelectItem(t, t.name()))
                .collect(Collectors.toList());
    }

    public int getSubjectId() {
        return subjectId;
    }

    public void setSubjectId(int subjectId) {
        this.subjectId = subjectId;
    }
}
