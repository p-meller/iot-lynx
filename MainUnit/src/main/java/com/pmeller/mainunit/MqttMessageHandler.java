package com.pmeller.mainunit;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;
import org.springframework.stereotype.Component;
@Component
public class MqttMessageHandler implements MessageHandler {

    @Autowired
    InputMessageRepository repository;

    @Override
    @ServiceActivator(inputChannel = "mqttInputChannel")
    public void handleMessage(Message<?> message) throws MessagingException {
        System.out.println(message.getPayload());
        String payload = (String) message.getPayload();
        InputMessageDto inputMessageDto = new InputMessageDto();
        inputMessageDto.setRawMessage(payload);
        inputMessageDto.setTopic(message.getHeaders().get("mqtt_receivedTopic", String.class));
        inputMessageDto.setStatus((byte) 0);
        repository.save(inputMessageDto);
    }
}