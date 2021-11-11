package com.pmeller.mainunit;

import lombok.Getter;
import lombok.Setter;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;

import java.util.List;

@Primary
@Configuration
@ConfigurationProperties(prefix = "mqtt")
@Getter
@Setter
public class MqttSettings {
    private String hostname;
    private int port;
    private String username;
    private String password;
    private String topics;
}