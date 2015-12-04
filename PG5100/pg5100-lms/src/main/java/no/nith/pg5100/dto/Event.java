package no.nith.pg5100.dto;

import javax.persistence.*;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import java.util.Date;

@Entity
@NamedQuery(name = "Event.getAll", query = "select e from Event e")
@SequenceGenerator(name = "SEQ_EVENT", initialValue = 50)
@Table(name = "EVENT")
@SecondaryTable(name = "EVENT_DETAILS", pkJoinColumns = @PrimaryKeyJoinColumn(name = "event_id"))
public class Event {


    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "SEQ_EVENT")
    private int id;

    @NotNull
    @Size(min = 5, max = 25, message = "An event's name must contain between 5 and 25 characters")
    private String title;

    @Size(max = 100, message = "An event's description must contain less than 100 characters")
    private String description;

    @Enumerated(EnumType.STRING)
    private EventType eventType;


    @Temporal(TemporalType.TIMESTAMP)
    @NotNull
    @Column(table = "EVENT_DETAILS")
    private Date startDateTime;

    @Temporal(TemporalType.TIMESTAMP)
    @NotNull
    @Column(table = "EVENT_DETAILS")
    private Date endDateTime;

 //   @Valid
    @NotNull
    @ManyToOne
    @JoinColumn(name = "FK_SUBJECT")
    private Subject subject;


    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public EventType getEventType() {
        return eventType;
    }

    public void setEventType(EventType eventType) {
        this.eventType = eventType;
    }

    public Date getStartDateTime() {
        return startDateTime;
    }

    public void setStartDateTime(Date startDateTime) {
        this.startDateTime = startDateTime;
    }

    public Date getEndDateTime() {
        return endDateTime;
    }

    public void setEndDateTime(Date endDateTime) {
        this.endDateTime = endDateTime;
    }

    public Subject getSubject() {
        return subject;
    }

    public void setSubject(Subject subject) {
        this.subject = subject;
    }

}
