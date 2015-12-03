package no.nith.pg5100.dto;

import javax.persistence.*;
import javax.validation.Valid;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;
import java.util.List;

@Entity
@NamedQuery(name = "Subject.getAll", query = "select s from Subject s")
@SequenceGenerator(name = "SEQ_SUBJECT", initialValue = 50)
public class Subject {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "SEQ_SUBJECT")
    private int id;

    @NotNull
    private String name;

    @Size(min = 0, max = 100)
    @ManyToMany(fetch = FetchType.EAGER)
    @JoinTable(name = "USR_SUB")
    private List<User> users;

    @ManyToOne
    @JoinColumn(name = "FK_LOCATION")
    @Valid
    private Location location;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<User> getUsers() {
        return users;
    }

    public void setUsers(List<User> users) {
        this.users = users;
    }

    public Location getLocation() {
        return location;
    }

    public void setLocation(Location location) {
        this.location = location;
    }
}
