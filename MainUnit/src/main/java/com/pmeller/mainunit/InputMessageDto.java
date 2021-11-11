package com.pmeller.mainunit;

import lombok.Getter;
import lombok.Setter;
import org.hibernate.annotations.CreationTimestamp;
import org.hibernate.annotations.UpdateTimestamp;

import javax.persistence.*;
import java.time.Instant;

@Entity
@Table(name = "INPUT_MESSAGE")
@Getter
@Setter
public class InputMessageDto {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ID", nullable = false)
    private Long id;

    @CreationTimestamp
    @Column(name = "CREATED_AT", nullable = false)
    private Instant createdAt;

    @UpdateTimestamp
    @Column(name = "UPDATED_AT", nullable = false)
    private Instant updatedAt;

    @Column(name = "TOPIC", nullable = false)
    private String topic;

    @Column(name = "RAW_MESSAGE", nullable = false)
    private String rawMessage;

    @Column(name = "STATUS", nullable = false)
    private byte status;
}
