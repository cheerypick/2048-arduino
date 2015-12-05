package no.nith.pg5100.dto;

import javax.persistence.*;
import javax.validation.constraints.NotNull;

@Entity
@NamedQuery(name = "Location.getAll", query = "select l from Location l")
@SequenceGenerator(name = "SEQ_LOCATION", initialValue = 50)
public class Location {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "SEQ_LOCATION")
    private int id;

    @NotNull
    private String room;
    @NotNull
    private String building;

    public int getId() {
        return id;
    }

    @Override
    public String toString() {
        return "Room " + room +
                ", building: " + building;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getRoom() {
        return room;
    }

    public void setRoom(String room) {
        this.room = room;
    }

    public String getBuilding() {
        return building;
    }

    public void setBuilding(String building) {
        this.building = building;
    }
}
