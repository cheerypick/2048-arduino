package no.nith.pg5100.dto;

import no.nith.pg5100.dto.constraint.ValidPassword;

import javax.persistence.*;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Pattern;
import java.util.List;

@Entity
@NamedQuery(name = "User.getAll", query = "select u from User u")
@SequenceGenerator(name = "SEQ_USER", initialValue = 50)
public class User {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "SEQ_USER")
    private int id;

    @NotNull
    @Pattern(regexp = "^\\S+@\\S+\\.\\S+$")
    private String email;

    @NotNull
    @ValidPassword
    private String password;

    @Enumerated(EnumType.STRING)
    private UserType userType;

    @ManyToMany(mappedBy = "users", fetch = FetchType.EAGER)
    private List<Subject> subjects;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public UserType getUserType() {
        return userType;
    }

    public void setUserType(UserType userType) {
        this.userType = userType;
    }

    public List<Subject> getSubjects() {
        return subjects;
    }

    public void setSubjects(List<Subject> subjects) {
        this.subjects = subjects;
    }
}
