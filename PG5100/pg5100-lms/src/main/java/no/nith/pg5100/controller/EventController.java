package no.nith.pg5100.controller;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import no.nith.pg5100.dto.Event;
import no.nith.pg5100.dto.EventType;
import no.nith.pg5100.infrastructure.event.EventDao;
import no.nith.pg5100.infrastructure.subject.SubjectDao;
import org.codehaus.jackson.JsonNode;
import org.codehaus.jackson.map.ObjectMapper;


import javax.annotation.PostConstruct;
import javax.enterprise.inject.Model;
import javax.faces.model.SelectItem;
import javax.inject.Inject;
import javax.json.JsonObject;
import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@Model
@Path("events")
public class EventController {

    @Inject
    private EventDao eventDao;

    @Inject
    private SubjectDao subjectDao;

    private Event event;

    private int selectedId;

    private int subjectId;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public List<Event> getAll() {
        List<Event> events =  eventDao.getAll().stream()
                .sorted((e1, e2) ->
                        e2.getStartDateTime() == null ? -1 : e1.getStartDateTime() == null ? 1 : e2.getStartDateTime().compareTo(e1.getStartDateTime()))
                .collect(Collectors.toList());
        System.out.println("getAllMessages(): found "+ events.size() + " message(s) on DB");
        return events;
    }

    @PostConstruct
    public void init() {
        event = new Event();
    }


    public void persist() {
        event.setSubject(subjectDao.findById(subjectId));
        eventDao.persist(event);
    }

    @POST()
    @Consumes(MediaType.APPLICATION_JSON)
    public void addEvent(JsonObject jsonObject) throws ParseException {

        Event e = new Event();
        String title = jsonObject.getString("title");
        DateFormat format = new SimpleDateFormat("mm-dd-yyyy");
        Date start = format.parse(jsonObject.getString("startDateTime"));
        Date end = format.parse(jsonObject.getString("startDateTime"));
        int subjectId = Integer.parseInt(jsonObject.getString("subject"));

        e.setTitle(title);
        e.setEndDateTime(end);
        e.setStartDateTime(start);
        e.setSubject(subjectDao.findById(subjectId));

        eventDao.persist(e);

    }

    public void fetchEvent() {
        event = eventDao.findById(selectedId);
    }

    @GET
    @Path("{id}")
    @Produces(MediaType.APPLICATION_JSON)
    public Event fetchEventById(@PathParam("id") int id) {

        return eventDao.findById(id);
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
