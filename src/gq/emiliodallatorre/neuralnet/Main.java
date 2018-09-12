package gq.emiliodallatorre.neuralnet;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.util.Arrays;
import java.util.Vector;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Hello World");
        primaryStage.setScene(new Scene(root, 300, 275));
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
        Vector<Integer> model = new Vector<>(Arrays.asList(7, 4, 2));
        Genetic genetic(100, model);
        int file = 0;
        while (quit!=true) {
            genetic.itinerate(50);
        }

        // TODO: Add interaction to save objects.
        for(auto& o : genetic.objects) {
            o.second.second.save_net("C:/Users/Riccardo-PC/Source/Repos/NNv0.0/NNv0.0/Saves/netsave#" + to_string(file) + ".net");
            file++;
        }
    }
}
